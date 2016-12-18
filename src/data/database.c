/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <stdlib.h>
#include <stdio.h>
#include <common/error.private.h>

#include "data/database.private.h"

pb_database * pb_open_database(const char * filename, int create, int auto_migrate) {
    pb_error(PB_E_NOT_IMPLEMENTED);
    return NULL;
}

int pb_migrate_database(pb_database * db) {
    db;       // Keep the IDE happy

    return 0; // Always succeed. We're on the lowest version, cannot load higher versions, so
              // there's no possibility to migrate anything.
}

void pb_close_database(pb_database * db) {
    if (!db)
        return;

    /* Close SQLite database */
    if (db->db) {
        if (sqlite3_close(db->db) != SQLITE_OK)
            fprintf(stderr, "WARNING: Could not close sqlite3 file!");
    }

    /* Free opaque struct memory */
    free(db);
}

int pb_execute_sql(pb_database * db, const char * sql) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_begin_transaction(pb_database * db) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_commit_transaction(pb_database * db) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_rollback_transaction(pb_database * db) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

unsigned int pb_get_current_version() {
    return 1;
}

unsigned int pb_get_database_version(pb_database * db) {
    pb_error(PB_E_NOT_IMPLEMENTED);
    return 0;
}

int pb_set_database_version(pb_database * db, unsigned int version) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}