/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_DATABASE_PRIVATE
#define PB_DATABASE_PRIVATE

#include <sqlite3.h>

#include "database.h"

struct pb_database {
    sqlite3 * connection;
};

/**
 * Sets the format version of a database WITHOUT doing a migration.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_set_database_version(pb_database * db, unsigned int version);

/**
 * Ensure writing to the database is possible
 * @return 1 when possible, otherwise 0 and pb_errno() will be set.
 */
int pb_write_possible(pb_database * db);

/**
 * Deletes a SQL row.
 * @param table The name of the table to delete the row from.
 * @param id The row's id column value
 * @return 0 on success, otherwise pb_errno()
 */
int pb_generic_delete(pb_database * db, const char * table, int id);

#endif