#include "common/error.private.h"

int pb_errno_v = 0;

int pb_errno() {
    return pb_errno_v;
}

int pb_error(int pb_errno) {
    return (pb_errno_v = pb_errno);
}

int pb_custom_error(const char * err_string) {
    return pb_error(PB_E_NOT_IMPLEMENTED);
}

const char * pb_error_str(int pb_errno) {
    return 0; // Not implemented yet
}