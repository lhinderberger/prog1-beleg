/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_DATABASE_PRIVATE
#define PB_DATABASE_PRIVATE

#include <sqlite3.h>

#include "database.h"

struct pb_database {
    sqlite3 * db;
};

/**
 * Sets the format version of a database WITHOUT doing a migration.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_set_database_version(pb_database * db, unsigned int version);

#endif