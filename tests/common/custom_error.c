#include "core.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "common/error.private.h"
#include "../test_utils.h"

int main() {
    if (pb_init())
        EXIT_PB_ERRNO();

    /* Trigger a custom error */
    char * test_error_message = "This is a custom test error message!";
    pb_custom_error(test_error_message);

    /* Check errno and message equality */
    assert(pb_errno() == PB_E_CUSTOM);
    assert(!strcmp(pb_error_str(pb_errno()), test_error_message));
    puts(pb_error_str(pb_errno()));

    return 0;
}