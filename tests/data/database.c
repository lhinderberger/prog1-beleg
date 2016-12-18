/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "../test_utils.h"

#include "data/database.private.h"

int main() {
    TEST_INIT();

    puts("Obtain temporary filename");
    char * temp_filename = tempnam(NULL, "pb_"); // Dangerous in production, okay in testing
    if (!temp_filename)
        exit_misc_error("ERROR: Could not retrieve temporary file name.");
    printf("Temporary file name: %s\n", temp_filename);

    puts("\nOpening non-existing database should fail");
    pb_database * db = NULL;
    db = pb_open_database(temp_filename, 0, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_EXISTS);

    puts("\nCreating a new database should work just fine");
    db = pb_open_database(temp_filename, 1, 1);
    TEST_ASSERT(db != NULL);

    puts("\nMake sure no newer version database can be opened");
    pb_set_database_version(db, pb_get_current_version() + 1);
    pb_close_database(db);
    db = pb_open_database(temp_filename, 0, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_VERSION);

    puts("\nCreating a database should fail on existing files");
    db = pb_open_database(temp_filename, 1, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_EXISTS);

    /* Clean up */
    remove(temp_filename);

    TEST_SUCCESS();
}