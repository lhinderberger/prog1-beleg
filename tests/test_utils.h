/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef TEST_UTILS
#define TEST_UTILS

#include <stdlib.h>
#include <stdio.h>

#include "core.h"
#include "common/error.h"
#include "data/material_item.h"

// Exits the test displaying a custom error message
void exit_misc_error(const char * message);

// Exits the test displaying the error message matching a specific pb_errno
void exit_pb_error(int pb_errno);

// Returns a temporary file name with error handling and debug output.
const char * get_temp_filename();

// Initializes an example material item
void init_example_item(pb_material_item * item);


/* ===== Macroes ===== */

#define EXIT_PB_ERRNO() exit_pb_error(pb_errno())

/**
 * Checks the assertion condition and exists the test, when it has failed
 * Note: This is intended to behave similar to standard assert(), but it will provide
 * more output, writes to stdout and does not require debug mode.
 * @param assertion A boolean expression to be asserted
 */
#define TEST_ASSERT(assertion) \
    puts("Test assertion: " #assertion); \
    if (!(assertion)) { \
        puts("Assertion failed!"); \
        exit(-1); \
    }

/* Macroes for main test structure, that is used in nearly every test */
#define TEST_INIT() \
if (pb_init()) \
    EXIT_PB_ERRNO();

#define TEST_SUCCESS() \
puts("\nTest passed!"); \
return 0;

#endif