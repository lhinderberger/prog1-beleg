/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_DATABASE
#define PB_DATABASE

typedef pb_database*;

/**
 * Opens or creates a database.
 * @param filename The unambiguous filename of the database, i.e. which database shall be
 *  opened or under which name it shall be created
 * @param create If this is zero, the file specified by filename MUST exist and be a valid database file.
 *  Otherwise, the target file must not exist and a new database will be created.
 * @param auto_migrate When not creating a database, set this to a nonzero value to automatically migrate the
 *  database format using pb_migrate_database() when it is using an outdated format.
 * @return NULL in case of error, otherwise a pointer to a newly created database. Take care to always close your
 *  database again using pb_close_database()!
 */
pb_database * pb_open_database(const char * filename, int create, int auto_migrate);

/**
 * Migrates a database to the current version
 * If a database uses an outdated format version (find out by comparing pb_get_database_version() vs.
 * pb_get_current_version()), you must migrate its format before being able to write to the database.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_migrate_database(pb_database * db);

/**
 * Closes a database.
 * This MUST be called when discarding a database previously obtained from this library,
 * otherwise data loss may or may not occur!
 */
void pb_close_database(pb_database * db);

/**
 * Executes the given SQL statement.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_execute_sql(pb_database * db, const char * sql);

/**
 * Begins a new SQL transaction.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_begin_transaction(pb_database * db);

/**
 * Commits the current SQL transaction.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_commit_transaction(pb_database * db);

/**
 * Rolls back the current SQL transaction.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_rollback_transaction(pb_database * db);

/**
 * Retrieves the most recent database version the currently loaded library can handle.
 *
 * @return A positive format version.
 */
unsigned int pb_get_current_version();

/**
 * Retrieves the format version of a database.
 * @return Zero on error, otherwise a positive format version.
 */
unsigned int pb_get_database_version(pb_database * db);

#endif