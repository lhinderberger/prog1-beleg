/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_ERROR_PRIVATE
#define PB_ERROR_PRIVATE

#include <sqlite3.h>
#include "error.h"

#define PB_CUSTOM_ERR_STR_LENGTH 512

// Global variables are all defined and initialized in core.c
extern int pb_errno_v;
extern char pb_custom_err_str[PB_CUSTOM_ERR_STR_LENGTH];

/**
 * Sets the error state.
 * Note: This will set the custom error string to a generic error string if pb_errno is PB_ERR_CUSTOM because
 * you are supposed to use pb_custom_error for custom errors.
 * @param pb_errno The error type to be written to pb_errno.
 * @return pb_errno
 */
int pb_error(int pb_errno);

/**
 * Sets the error state to a custom error and sets a custom error string.
 * Exclusively use this function instead of pb_error(PB_ERR_CUSTOM) to trigger custom errors!
 * @return PB_E_CUSTOM
 */
int pb_custom_error(const char * err_string);

/**
 * Sets the error state to PB_E_DB_BACKEND and prints the SQLite error message to stderr.
 * @return PB_E_DB_BACKEND
 */
int pb_sqlite_error(sqlite3 * db);

/**
 * Prints an error message to stderr, containing details about what failed in SQLite but not setting
 * pb_errno.
 */
void pb_sqlite_warning(sqlite3 * db);

#endif