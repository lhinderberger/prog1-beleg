/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <memory.h>

#include "core.h"
#include "common/error.private.h"

int pb_errno_v;
char pb_custom_err_str[512];

int pb_init() {
    /* Initialize global error variables */
    pb_errno_v = 0;
    memset(pb_custom_err_str, 0, PB_CUSTOM_ERR_STR_LENGTH);

    return 0;
}