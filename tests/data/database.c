/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include "../test_utils.h"

#include "data/database.private.h"

int main() {
    TEST_INIT();

    /* Obtain temporary filename */
    char * temp_filename = tempnam(NULL, NULL); // Dangerous in production, okay in testing
    if (!temp_filename)
        exit_misc_error("ERROR: Could not retrieve temporary file name.");

    /* Opening non-existing database should fail */
    pb_database * db = NULL;
    db = pb_open_database(temp_filename, 0, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_FOPEN);

    /* Creating a new database should work just fine */
    db = pb_open_database(temp_filename, 1, 1);
    TEST_ASSERT(db != NULL);

    /* Make sure no newer version database can be opened */
    pb_set_database_version(db, pb_get_current_version() + 1);
    pb_close_database(db);
    db = pb_open_database(temp_filename, 0, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_VERSION);

    /* Creating a database should fail on existing files */
    db = pb_open_database(temp_filename, 1, 1);
    TEST_ASSERT(db == NULL);
    TEST_ASSERT(pb_errno() == PB_E_FOPEN);

    /* Clean up */
    remove(temp_filename);

    TEST_SUCCESS();
}