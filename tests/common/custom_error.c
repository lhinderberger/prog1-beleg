/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <string.h>
#include <stdio.h>

#include "../test_utils.h"
#include "common/error.private.h"

int main() {
    TEST_INIT();

    /* Test error initialization */
    puts("Initialization test...");
    TEST_ASSERT(pb_errno() == 0);

    /* Trigger a custom error */
    puts("\nCustom error test...");
    char * test_error_message = "This is a custom test error message!";
    printf("Custom error message (to API): %s\n", test_error_message);
    pb_custom_error(test_error_message);

    /* Check errno and message equality */
    const char * err_string = pb_error_str(pb_errno());
    TEST_ASSERT(err_string != NULL);
    TEST_ASSERT(pb_errno() == PB_E_CUSTOM);
    TEST_ASSERT(!strcmp(err_string, test_error_message));
    printf("Custom error message (from API): %s\n", err_string);

    /* Test error clearing */
    puts("\nCleanup test...");
    pb_clear_error();
    TEST_ASSERT(pb_errno() == 0);

    TEST_SUCCESS();
}