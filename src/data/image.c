/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdlib.h>
#include <memory.h>
#include <data/query.private.h>
#include <data/database.private.h>

#include "common/error.private.h"
#include "data/image.h"
#include "data/query.h"

void * pb_image_retrieve(pb_database * db, int id, int * bytes_out) {
    pb_clear_error();

    if (!db || !bytes_out) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    /* Prepare query to lookup image */
    pb_query * query = pb_query_prepare(db, "SELECT bytes FROM images WHERE id = :id" , -1);
    if (!query)
        return NULL;

    /* Bind id parameter */
    if (pb_query_bind_int(query, 1, id) != 0) {
        pb_query_discard(query);
        return NULL;
    }

    /* Execute */
    if (!pb_query_step(query)) {
        pb_query_discard(query);
        if (!pb_errno())
            pb_error(PB_E_NOTFOUND);
        return NULL;
    }

    /* Retrieve data blob */
    const void * query_result = pb_query_column_blob(query, 0, bytes_out);
    if (!query_result) { // When empty or on error; On error query_column_blob will set pb_errno()
        pb_query_discard(query);
        return NULL;
    }

    /* Ensure bytes_out is not negative */
    if (bytes_out < 0) {
        pb_query_discard(query);
        pb_custom_error("Got negative byte count from SQLite blob retrieval");
        return NULL;
    }

    /* Copy data blob */
    void * out = malloc(*bytes_out * sizeof(void));
    if (!out) {
        pb_query_discard(query);
        pb_error(PB_E_MALLOC);
        return NULL;
    }
    memcpy(out, query_result, (size_t)*bytes_out);

    /* Clean up and return */
    pb_query_discard(query);
    return out;
}

int pb_image_delete(pb_database * db, int id) {
    return pb_generic_delete(db, "images", id);
}

int pb_image_save(pb_database * db, int id, int update, void * data, int data_bytes) {
    pb_clear_error();

    /* Validate input */
    if (!pb_write_possible(db))
        return 0;

    if (update && !id) {
        pb_error(PB_E_UPDATE_ID_MISSING);
        return 0;
    }

    /* Prepare query to save image */
    pb_query * query = NULL;
    if (id)
        query = pb_query_prepare(db, update ? "UPDATE images SET bytes = :data WHERE id = :id" : "INSERT INTO images(bytes, id) VALUES(:data, :id)" , -1);
    else
        query = pb_query_prepare(db, "INSERT INTO images(bytes) VALUES(:data)", -1);
    if (!query)
        return 0;

    /* Bind data parameter */
    if (data && data_bytes > 0) {
        if (pb_query_bind_blob(query, 1, data, data_bytes) != 0) {
            pb_query_discard(query);
            return pb_errno();
        }
    }
    else {
        if (pb_query_bind_null(query, 1) != 0) {
            pb_query_discard(query);
            return pb_errno();
        }
    }

    /* Bind id parameter */
    if (id && pb_query_bind_int(query, 2, id) != 0) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Execute */
    if (!pb_query_step(query) && pb_errno()) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Clean up and return image id */
    pb_query_discard(query);
    query = NULL;
    return id ? id : (int)sqlite3_last_insert_rowid(db->connection);
}

void pb_image_free(void * blob) {
    pb_clear_error();
    if (blob)
        free(blob);
}

