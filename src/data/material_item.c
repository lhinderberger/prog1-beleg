/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdlib.h>
#include <memory.h>

#include "common/error.private.h"
#include "data/database.private.h"
#include "data/material_item.h"

#define TYPE_INT 1
#define TYPE_STRING 2

pb_material_item * pb_mat_item_create() {
    pb_clear_error();

    pb_material_item * result = (pb_material_item*)malloc(sizeof(pb_material_item));
    if (!result) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }
    memset(result, 0, sizeof(pb_material_item));
    return result;
}

pb_material_item * pb_mat_item_retrieve(pb_database * db, int id) {
    pb_clear_error();

    /* Validate input */
    if (!db) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    if (id < 1) {
        pb_error(PB_E_RANGE);
        return NULL;
    }

    /* Build query */
    pb_query * query = pb_query_prepare(db, "SELECT id, name, article_no, n_stock, image_id FROM material_items WHERE id = :id", -1);
    if (!query)
        return NULL;

    /* Bind ID parameter */
    if (pb_query_bind_int(query, 1, id) != 0) {
        pb_query_discard(query);
        return NULL;
    }

    /* Execute query, pass on result */
    if (!pb_query_step(query)) {
        if (!pb_errno())
            pb_error(PB_E_NOTFOUND);
        pb_query_discard(query);
        return NULL;
    }
    int mapping[] = {
        PB_MAT_ITEM_VAR_ID, 0,
        PB_MAT_ITEM_VAR_NAME, 1,
        PB_MAT_ITEM_VAR_ARTICLE_NO, 2,
        PB_MAT_ITEM_VAR_N_STOCK, 3,
        PB_MAT_ITEM_VAR_IMAGE_ID, 4,
        0
    };
    pb_material_item * result = pb_mat_item_from_query(query, mapping);
    // Note: No need for checking error from mat_item_from_query, as we would discard the query
    // and return NULL either way.

    /* Cleanup and return */
    pb_query_discard(query);
    return result;
}

pb_material_item * pb_mat_item_from_query(pb_query * query, const int * column_mapping) {
    pb_clear_error();

    if (!query || !column_mapping) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    /* Create new material item */
    pb_material_item * result = pb_mat_item_create();
    if (!result)
        return NULL;

    /* Iterate over column mapping and fill material item */
    while (*column_mapping != 0) {
        void * target = NULL;
        int type = 0;
        int col_index = 0;
        int maxlength = 0;

        /* Determine what to retrieve (where to store, what type) */
        switch (*column_mapping) {
            case PB_MAT_ITEM_VAR_ID:
            case PB_MAT_ITEM_VAR_IMAGE_ID:
            case PB_MAT_ITEM_VAR_N_STOCK:
                type = TYPE_INT;
                break;
            case PB_MAT_ITEM_VAR_NAME:
            case PB_MAT_ITEM_VAR_ARTICLE_NO:
                type = TYPE_STRING;
                break;
            default:
                pb_error(PB_E_RANGE);
                pb_mat_item_free(result);
                return NULL;
        }

        switch (*column_mapping) {
            case PB_MAT_ITEM_VAR_ID:
                target = &result->id;
                break;
            case PB_MAT_ITEM_VAR_IMAGE_ID:
                target = &result->image_id;
            case PB_MAT_ITEM_VAR_N_STOCK:
                target = &result->n_stock;
                break;
            case PB_MAT_ITEM_VAR_NAME:
                target = &result->name;
                maxlength = 64;
                break;
            case PB_MAT_ITEM_VAR_ARTICLE_NO:
                target = &result->article_no;
                maxlength = 32;
                break;
            default:
                pb_error(PB_E_RANGE);
                pb_mat_item_free(result);
                return NULL;
        }

        /* Determine the column index */
        col_index = *(++column_mapping);
        if (col_index <= 0) {
            pb_error(PB_E_RANGE);
            pb_mat_item_free(result);
            return NULL;
        }

        /* Retrieve column */
        if (type == TYPE_INT) {
            int * int_target = (int*)target;

            *int_target = pb_query_column_int(query, col_index);
            if (!*int_target && pb_errno() != 0) {
                pb_mat_item_free(result);
                return NULL;
            }
        }
        else if (type == TYPE_STRING) {
            /* Retrieve value */
            const char * string_out = pb_query_column_string(query, col_index);
            if (!string_out) {
                pb_mat_item_free(result);
                return NULL;
            }

            /* Make sure length does not exceed string length limit. This is important for security! */
            if (strlen(string_out) > maxlength) {
                pb_mat_item_free(result);
                pb_error(PB_E_TOOLONG);
                return NULL;
            }

            /* Copy string */
            strcpy((char*)target, string_out);
        }

        /* Continue to next mapping */
        column_mapping++;
    }

    return result;
}

void pb_mat_item_free(pb_material_item * item) {
    pb_clear_error();

    if (item)
        free(item);
}

int pb_mat_item_save(pb_database * db, pb_material_item * item, int update) {
    pb_clear_error();

    /* Validate input */
    if (!pb_write_possible(db))
        return 0;

    if (!item) {
        pb_error(PB_E_NULLPTR);
        return 0;
    }

    if (update && !item->id) {
        pb_error(PB_E_UPDATE_ID_MISSING);
        return 0;
    }

    /* Prepare query to save material item */
    pb_query * query = NULL;
    if (item->id)
        query = pb_query_prepare(
            db,
            update ?
            "UPDATE material_items SET name = :name, article_no = :article_no, n_stock = :n_stock, image_id = :image_id WHERE id = :id" :
            "INSERT INTO material_items(name, article_no, n_stock, image_id, id) VALUES(:name, :article_no, :n_stock, :image_id, :id)",
            -1
        );
    else
        query = pb_query_prepare(
            db,
            "INSERT INTO material_items(name, article_no, n_stock, image_id) VALUES(:name, :article_no, :n_stock, :image_id)",
            -1
        );
    if (!query)
        return 0;

    /* Bind data parameters */
    if (pb_query_bind_string(query, 1, item->name, -1)) {
        pb_query_discard(query);
        return 0;
    }

    if (pb_query_bind_string(query, 2, item->article_no, -1)) {
        pb_query_discard(query);
        return 0;
    }

    if (pb_query_bind_int(query, 3, item->n_stock)) {
        pb_query_discard(query);
        return 0;
    }

    if (pb_query_bind_int(query, 4, item->image_id)) {
        pb_query_discard(query);
        return 0;
    }

    /* Bind id parameter */
    if (item->id && pb_query_bind_int(query, 5, item->id)) {
        pb_query_discard(query);
        return 0;
    }

    /* Execute */
    if (!pb_query_step(query) && pb_errno()) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Clean up and return material item id */
    pb_query_discard(query);
    query = NULL;
    return item->id ? item->id : (int)sqlite3_last_insert_rowid(db->connection);
}

int pb_mat_item_delete(pb_database * db, int id) {
    return pb_generic_delete(db, "material_items", id);
}