/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdio.h>
#include <malloc.h>

#include "data/material_item.private.h"
#include "logic/items_query.h"
#include "common/error.private.h"

int mapping[] = {
        PB_MAT_ITEM_VAR_ID, 0,
        PB_MAT_ITEM_VAR_NAME, 1,
        PB_MAT_ITEM_VAR_ARTICLE_NO, 2,
        PB_MAT_ITEM_VAR_N_STOCK, 3,
        PB_MAT_ITEM_VAR_IMAGE_ID, 4,
        0
};

int retrieve_query_results(pb_query * query, pb_material_item_buffer buf_out) {
    int n = 0;
    while (pb_query_step(query)) {
        if (pb_mat_item_from_query_ptr(query, buf_out[n], mapping) != 0) {
            pb_query_discard(query);
            return -1;
        }
        n++;
    }

    pb_query_discard(query);
    if (pb_errno() != 0) // There was an error during querying, abort
        return -1;

    return n;
}

pb_material_item_buffer pb_create_mat_item_buffer(int size) {
    if (size < 0) {
        pb_error(PB_E_RANGE);
        return NULL;
    }

    /* Create outer array of pointers */
    pb_material_item ** result = (pb_material_item**)calloc((size_t)size + 1, sizeof(pb_material_item*));
    if (!result) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }

    /* Create inner structures */
    for (int i = 0; i < size; i++) {
        result[i] = (pb_material_item*)calloc(1, sizeof(pb_material_item));
        if (!result[i]) {
            pb_error(PB_E_MALLOC);
            pb_free_mat_item_buffer(result); // This can handle incompletely initialized buffers, due to the use of calloc above.
            return NULL;
        }
    }

    return result;
}

void pb_free_mat_item_buffer(pb_material_item_buffer buffer) {
    if (!buffer)
        return;

    for (int i = 0; buffer[i]; i++)
        free(buffer[i]);

    free(buffer);
}

int pb_find_mat_items(pb_database * db, pb_material_item_buffer buf_out, const char * search_string, int field, int count_mode, int offset, int limit) {
    pb_clear_error();

    /* Validate input */
    if (!db || (!count_mode && !buf_out) || !search_string) {
        pb_error(PB_E_NULLPTR);
        return -1;
    }

    if (!count_mode && (offset < 0 || limit < 0)) {
        pb_error(PB_E_RANGE);
        return -1;
    }

    const char * search_column = NULL;
    switch (field) {
        case PB_MAT_ITEM_VAR_ARTICLE_NO:
            search_column = "article_no";
            break;
        case PB_MAT_ITEM_VAR_NAME:
            search_column = "name";
            break;
        default:
            pb_custom_error("Unsupported search field!");
            return -1;
    }

    /* Prepare query */
    char query_sql[256];
    if (sprintf(query_sql, "SELECT id, name, article_no, n_stock, image_id%s FROM material_items WHERE %s LIKE '%%' || :search_string || '%%' %s", count_mode ? ", COUNT(*)" : "", search_column, count_mode ? "" : "LIMIT :limit OFFSET :offset") < 0) {
        pb_error(PB_E_TOOLONG);
        return -1;
    }
    pb_query * query = pb_query_prepare(db, query_sql, -1);
    if (!query)
        return -1;

    /* Bind parameters */
    if (pb_query_bind_string(query, 1, search_string, -1) != 0) {
        pb_query_discard(query);
        return -1;
    }

    if (!count_mode) {
        if (pb_query_bind_int(query, 2, limit) != 0) {
            pb_query_discard(query);
            return -1;
        }

        if (pb_query_bind_int(query, 3, offset) != 0) {
            pb_query_discard(query);
            return -1;
        }
    }

    /* Execute query */
    if (count_mode) {
        if (!pb_query_step(query)) {
            pb_query_discard(query);
            return -1;
        }

        int result = pb_query_column_int(query, 5);
        pb_query_discard(query);
        if (pb_errno() != 0)
            return -1;

        return result;
    }
    else {
        return retrieve_query_results(query, buf_out);
    }
}

int pb_list_mat_items(pb_database * db, pb_material_item_buffer buf_out, int sort_field, int sort_ascending, int offset, int limit) {
    pb_clear_error();

    /* Validate input */
    if (!db || !buf_out) {
        pb_error(PB_E_NULLPTR);
        return -1;
    }

    if (offset < 0 || limit < 0) {
        pb_error(PB_E_RANGE);
        return -1;
    }

    const char * sort_field_name = NULL;
    switch (sort_field) {
        case PB_MAT_ITEM_VAR_ARTICLE_NO:
            sort_field_name = "article_no";
            break;
        case PB_MAT_ITEM_VAR_ID:
            sort_field_name = "id";
            break;
        case PB_MAT_ITEM_VAR_IMAGE_ID:
            sort_field_name = "image_id";
            break;
        case PB_MAT_ITEM_VAR_N_STOCK:
            sort_field_name = "n_stock";
            break;
        case PB_MAT_ITEM_VAR_NAME:
            sort_field_name = "name";
            break;
        default:
            pb_custom_error("Unsupported sort field!");
            return -1;
    }

    /* Prepare query */
    char query_sql[256];
    if (sprintf(query_sql, "SELECT id, name, article_no, n_stock, image_id FROM material_items ORDER BY %s %s LIMIT :limit OFFSET :offset", sort_field_name, sort_ascending ? "ASC" : "DESC") < 0) {
        pb_error(PB_E_TOOLONG);
        return -1;
    }
    pb_query * query = pb_query_prepare(db, query_sql, -1);
    if (!query)
        return -1;

    /* Bind parameters */
    if (pb_query_bind_int(query, 1, limit) != 0) {
        pb_query_discard(query);
        return -1;
    }

    if (pb_query_bind_int(query, 2, offset) != 0) {
        pb_query_discard(query);
        return -1;
    }

    /* Execute query */
    return retrieve_query_results(query, buf_out);
}

int pb_count_mat_items(pb_database * db) {
    pb_clear_error();
    if (!db) {
        pb_error(PB_E_NULLPTR);
        return -1;
    }

    /* Prepare and execute query */
    pb_query * query = pb_query_prepare(db, "SELECT COUNT(*) FROM material_items;", -1);
    if (!query)
        return -1;
    if (!pb_query_step(query))
        return -1;

    /* Retrieve result and clean up */
    int result = pb_query_column_int(query, 0);
    pb_query_discard(query);
    if (pb_errno() != 0)
        return -1;

    return result;
}