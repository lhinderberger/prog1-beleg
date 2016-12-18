/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include "common/error.private.h"

#include <string.h>
#include <stdio.h>

int pb_errno() {
    return pb_errno_v;
}

int pb_error(int pb_errno) {
    return (pb_errno_v = pb_errno);
}

void pb_clear_error() {
    pb_errno_v = 0;
    memset(pb_custom_err_str, 0, PB_CUSTOM_ERR_STR_LENGTH);
}

int pb_custom_error(const char * err_string) {
    // Only copy as much characters as our buffer can handle.
    // Always ensure to null-terminate the error string!
    strncpy(pb_custom_err_str, err_string, PB_CUSTOM_ERR_STR_LENGTH - 1);
    pb_custom_err_str[PB_CUSTOM_ERR_STR_LENGTH - 1] = 0;

    // Pass on control to regular pb_error()
    return pb_error(PB_E_CUSTOM);
}

int pb_sqlite_error(sqlite3 * db) {
    pb_sqlite_warning(db);
    return pb_error(PB_E_DB_BACKEND);
}

void pb_sqlite_warning(sqlite3 * db) {
    fprintf(stderr, "Error in SQLite.\nDetails: %s\n", sqlite3_errmsg(db));
}

const char * pb_error_str(int pb_errno) {
    static const char * fallback_msg = "An unknown error occurred!";

    switch (pb_errno) {
        case PB_E_EXISTS:     return "Error opening file! Check if file exists / check permissions.";
        case PB_E_VERSION:    return "Database version mismatch. Tried to open newer version database / write to older version?";
        case PB_E_DB_BACKEND: return "Database backend error";
        case PB_E_MALLOC:     return "Error allocating memory";
        case PB_E_RANGE:      return "Argument / variable / query result out of range";
        case PB_E_NULLPTR:    return "Program corrupted: Null pointer passed to pb-core!";

        // We do not want to return empty error strings.
        case PB_E_CUSTOM:   return (pb_custom_err_str[0]) ? pb_custom_err_str : fallback_msg;

        default: return fallback_msg;
    }
}