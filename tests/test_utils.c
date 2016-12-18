/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/error.h"

void exit_misc_error(const char * message) {
    printf("ERROR: %s\n", message);
    exit(-1);
}

void exit_pb_error(int pb_errno) {
    printf("ERROR in pb-core:\n%s\n", pb_error_str(pb_errno));
    exit(-1);
}