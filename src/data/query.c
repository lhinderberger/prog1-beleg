/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <stdlib.h>
#include <memory.h>

#include "common/error.private.h"
#include "data/database.private.h"
#include "data/query.private.h"

#define query_corrupted(query) (!query || !query->statement || !query->connection)

pb_query * pb_query_prepare(pb_database * db, const char * sql, int sql_bytes) {
    pb_clear_error();

    if (!db || !sql || !db->connection) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    /* Create opaque struct */
    pb_query * query = malloc(sizeof(pb_query));
    if (!query) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }

    memset(query, 0, sizeof(pb_query));
    query->connection = db->connection;

    /* Prepare statement */
    if (sqlite3_prepare_v2(query->connection, sql, sql_bytes, &query->statement, NULL)) {
        pb_sqlite_error(query->connection);
        free(query);
        return NULL;
    }

    return query;
}

int pb_query_bind_string(pb_query * query, int param_index, const char * value, int value_bytes) {
    if (query_corrupted(query) || !value)
        return pb_error(PB_E_NULLPTR);

    if (sqlite3_bind_text(query->statement, param_index, value, value_bytes, NULL) != SQLITE_OK)
        return pb_sqlite_error(query->connection);

    return 0;
}

const char * pb_query_column_string(pb_query * query, int column_index) {
    pb_clear_error();

    if (query_corrupted(query)) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    const char * column = (const char*)sqlite3_column_text(query->statement, column_index);
    if (!column)
        pb_sqlite_error(query->connection);

    return column;
}

int pb_query_step(pb_query * query) {
    if (query_corrupted(query))
        return pb_error(PB_E_NULLPTR);

    int res = sqlite3_step(query->statement);
    if (res == SQLITE_OK || res == SQLITE_ROW)        // Row obtained, statement still valid
        return 1;
    else if (res == SQLITE_DONE) // Last row obtained
        return 0;
    else {                       // Something went wrong
        pb_sqlite_error(query->connection);
        return 0;
    }
}

void pb_query_discard(pb_query * query) {
    if (!query)
        return;

    if (query->statement) {
        if (sqlite3_finalize(query->statement) != SQLITE_OK)
            pb_sqlite_warning(query->connection);
    }
}