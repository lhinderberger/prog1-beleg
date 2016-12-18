/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include "common/error.private.h"

#include <string.h>

int pb_errno() {
    return pb_errno_v;
}

int pb_error(int pb_errno) {
    return (pb_errno_v = pb_errno);
}

int pb_custom_error(const char * err_string) {
    // Only copy as much characters as our buffer can handle.
    // Always ensure to null-terminate the error string!
    strncpy(pb_custom_err_str, err_string, PB_CUSTOM_ERR_STR_LENGTH - 1);
    pb_custom_err_str[PB_CUSTOM_ERR_STR_LENGTH - 1] = 0;

    // Pass on control to regular pb_error()
    return pb_error(PB_E_CUSTOM);
}

const char * pb_error_str(int pb_errno) {
    static const char * fallback_msg = "An unknown error occurred!";

    switch (pb_errno) {
        case PB_E_FOPEN:    return "Error opening file! Check if file exists / check permissions.";
        case PB_E_VERSION:  return "Database version mismatch. Tried to open newer version database / write to older version?";

        // We do not want to return empty error strings.
        case PB_E_CUSTOM:   return (pb_custom_err_str[0]) ? pb_custom_err_str : fallback_msg;

        default: return fallback_msg;
    }
}