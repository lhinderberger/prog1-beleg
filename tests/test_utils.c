#include "test_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/error.h"

void exit_pb_error(int pb_errno) {
    fprintf(stderr, "ERROR in pb-core:\n%s\n", pb_error_str(pb_errno));
    exit(-1);
}