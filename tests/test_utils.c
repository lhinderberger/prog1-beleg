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

const char * get_temp_filename() {
    puts("Obtain temporary filename");
    char * temp_filename = tempnam(NULL, "pb_"); // Dangerous in production, okay in testing
    if (!temp_filename)
        exit_misc_error("ERROR: Could not retrieve temporary file name.");
    printf("Temporary file name: %s\n", temp_filename);

    return temp_filename;
}