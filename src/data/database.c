/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <stdlib.h>
#include <stdio.h>
#include <common/error.private.h>
#include <memory.h>

#include "data/database.private.h"
#include "data/query.h"

const char * init_sql =
    "CREATE TABLE meta("
        "name CHAR(16) PRIMARY KEY,"
        "value TEXT"
    ");"
    "CREATE TABLE images("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "bytes BLOB"
    ");"
    "CREATE TABLE material_items("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "name CHAR(64),"
        "article_no CHAR(32),"
        "image_id INT,"
        "n_stock INT,"
        "FOREIGN KEY(image_id) REFERENCES images(id)"
    ");"
    "INSERT INTO meta VALUES ('version', '0');";

/* Local utility functions */
int file_exists(const char * filename) {
    FILE * f = fopen(filename, "r");
    if (!f)
        return 0;
    fclose(f);
    return 1;
}

/* Interface implementation */
pb_database * pb_open_database(const char * filename, int create, int auto_migrate) {
    pb_clear_error();

    /* Validate input */
    if (!filename) {
        pb_error(PB_E_NULLPTR);
        return NULL;
    }

    /* Check file exists constraints */
    int exists = file_exists(filename);
    if ((exists && create) || (!exists && !create)) {
        pb_error(PB_E_EXISTS);
        return NULL;
    }

    /* Create opaque struct */
    pb_database * db = (pb_database*)malloc(sizeof(pb_database));
    if (!db) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }
    memset(db, 0, sizeof(pb_database));

    /* Open SQLite database */
    int res = sqlite3_open_v2(
            filename,
            &db->connection,
            SQLITE_OPEN_READWRITE | (create ? SQLITE_OPEN_CREATE : 0),
            NULL
    );
    if (res != SQLITE_OK) {
        free(db);
        fprintf(stderr, "Error in SQLite: %s\n", sqlite3_errstr(res));
        pb_error(PB_E_DB_BACKEND);
        return NULL;
    }

    /* Initialize database if freshly created */
    if (create) {
        if (
            pb_execute_sql(db, init_sql) != 0 ||
            pb_set_database_version(db, pb_get_current_version()) != 0
        ){
            pb_close_database(db);
            return NULL;
        }
    }
    /* Otherwise check version */
    else {
        unsigned int version = pb_get_database_version(db);
        if (!version) {
            pb_close_database(db);
            return NULL; // pb_errno is set by pb_get_database_version
        }

        if (version > pb_get_current_version()) {
            pb_close_database(db);
            pb_error(PB_E_VERSION);
            return NULL;
        }
        else if (version < pb_get_current_version()) {
            if (auto_migrate && pb_migrate_database(db) != 0) {
                pb_close_database(db);
                return NULL;
            }
        }
    }

    /* Enable foreign key constraints */
    pb_execute_sql(db, "PRAGMA foreign_keys = ON;");

    /* And we're done */
    return db;
}

int pb_migrate_database(pb_database * db) {
    pb_clear_error();
    return 0; // Always succeed. We're on the lowest version, cannot load higher versions, so
              // there's no possibility to migrate anything.
}

void pb_close_database(pb_database * db) {
    pb_clear_error();
    if (!db)
        return;

    /* Close SQLite database */
    if (db->connection) {
        if (sqlite3_close(db->connection) != SQLITE_OK)
            pb_sqlite_warning(db->connection);
    }

    /* Free opaque struct memory */
    free(db);
}

int pb_execute_sql(pb_database * db, const char * sql) {
    pb_clear_error();
    if (!db || !sql || !db->connection)
        return pb_error(PB_E_NULLPTR);

    int res = sqlite3_exec(db->connection, sql, NULL, NULL, NULL);
    if (res == SQLITE_CONSTRAINT)
        return pb_error(PB_E_CONSTRAINT);
    else if (res != SQLITE_OK)
        return pb_sqlite_error(db->connection);

    return 0;
}

int pb_begin_transaction(pb_database * db) {
    pb_clear_error();
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_commit_transaction(pb_database * db) {
    pb_clear_error();
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

int pb_rollback_transaction(pb_database * db) {
    pb_clear_error();
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

unsigned int pb_get_current_version() {
    pb_clear_error();
    return 1;
}

unsigned int pb_get_database_version(pb_database * db) {
    pb_clear_error();

    /* Create query to retrieve version field */
    pb_query * query = pb_query_prepare(db, "SELECT value FROM meta WHERE name = 'version'", -1);
    if (!query)
        return 0;

    /* Step and retrieve */
    if (!pb_query_step(query)) {
        if (!pb_errno()) // In case this is not a SQLite error, but actually no version found
            pb_custom_error("Version not found in database - Wrong file?");
        pb_query_discard(query);
        return 0;
    }
    const char * version_str = pb_query_column_string(query, 0);
    if (!version_str) {
        pb_query_discard(query);
        return 0;
    }

    /* Convert to integer */
    int version = atoi(version_str);
    if (version < 0) {
        pb_error(PB_E_RANGE);
        pb_query_discard(query);
        return 0;
    }

    /* Cleanup and return */
    pb_query_discard(query);
    return (unsigned int)version;
}

int pb_set_database_version(pb_database * db, unsigned int version) {
    pb_clear_error();

    /* Build version string */
    char version_str[32];
    if (sprintf(version_str, "%u", version) != 1)
        return pb_custom_error("sprintf failed!");

    /* Create query to update version field */
    pb_query * query = pb_query_prepare(db, "UPDATE meta SET value = :value WHERE name = 'version'", -1);
    if (!query)
        return pb_errno();

    /* Bind parameter and execute */
    if (pb_query_bind_string(query, 1, version_str, -1) != 0) {
        pb_query_discard(query);
        return pb_errno();
    }
    if (!pb_query_step(query) && pb_errno() != 0) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Clean up and return */
    pb_query_discard(query);
    return 0;
}

int pb_write_possible(pb_database * db) {
    if (!db) {
        pb_error(PB_E_NULLPTR);
        return 0;
    }

    if (pb_get_database_version(db) != pb_get_current_version()) {
        pb_error(PB_E_VERSION);
        return 0;
    }

    return 1;
}

int pb_generic_delete(pb_database * db, const char * table, int id) {
    pb_clear_error();

    /* Validate input */
    if (!pb_write_possible(db))
        return pb_errno();

    if (strcmp(table, "images") && strcmp(table, "material_items"))
        return pb_error(PB_E_RANGE); // White-list allowed table names to avoid SQL injection

    /* Prepare query to delete image */
    char sql[512];
    if (sprintf(sql, "DELETE FROM %s WHERE id = :id", table) < 0)
        return pb_error(PB_E_TOOLONG);

    pb_query * query = pb_query_prepare(db, sql, -1);
    if (!query)
        return pb_errno();

    /* Bind id parameter */
    if (pb_query_bind_int(query, 1, id) != 0) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Execute */
    if (!pb_query_step(query) && pb_errno()) {
        pb_query_discard(query);
        return pb_errno();
    }

    /* Clean up and return */
    pb_query_discard(query);
    return 0;
}