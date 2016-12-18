/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdlib.h>
#include <memory.h>

#include "common/error.private.h"
#include "data/image.h"
#include "data/query.h"

void * pb_image_retrieve(pb_database * db, int id, int * bytes_out) {
    pb_clear_error();

    if (!db | !bytes_out) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    /* Prepare query to lookup image */
    pb_query * query = pb_query_prepare(db, "SELECT data FROM images WHERE id = :id" , -1);
    if (!query)
        return NULL;

    /* Bind id parameter */
    if (!pb_query_bind_int(query, 1, id)) {
        pb_query_discard(query);
        return NULL;
    }

    /* Execute */
    if (!pb_query_step(query)) {
        if (!pb_errno())
            pb_error(PB_E_NOTFOUND);
        pb_query_discard(query);
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
    memcpy(out, query_result, (size_t)*bytes_out);

    /* Clean up and return */
    pb_query_discard(query);
    return out;
}

int pb_image_delete(pb_database * db, int id) {
    pb_clear_error();
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_image_save(pb_database * db, int id, int overwrite_existing, void * data, int data_bytes) {
    pb_clear_error();
    pb_error(PB_E_NOT_IMPLEMENTED);
    return 0;
}

void pb_image_free(void * blob) {
    pb_clear_error();
    if (blob)
        free(blob);
}

