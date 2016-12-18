/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <memory.h>
#include <data/database.private.h>
#include "../test_utils.h"

#include "data/database.h"
#include "data/image.h"

int main() {
    TEST_INIT();

    puts("Create database");
    const char * temp_filename = get_temp_filename();
    pb_database * db = pb_open_database(temp_filename, 1, 0);
    if (!db)
        EXIT_PB_ERRNO();

    puts("\nCreate image");
    const char * helloworld = "Hello World!"; // We'll cheat here and use a string instead of an image.
    int len = (int)strlen(helloworld) - 1; // We deliberately leave out the NULL terminator here for testing purposes.
    int image_id = pb_image_save(db, 0, 0, &helloworld, len);
    TEST_ASSERT(image_id != 0);
    printf("image_id: %d\n", image_id);

    puts("\nReading back");
    int len_out = 0;
    void * data_out = pb_image_retrieve(db, image_id, &len_out);
    TEST_ASSERT(data_out != NULL);
    TEST_ASSERT(len_out == len);

    puts("\nComparing");
    TEST_ASSERT(!memcmp(helloworld, data_out, (size_t)len));
    pb_image_free(data_out);
    data_out = NULL;

    puts("\nDeleting");
    TEST_ASSERT(!pb_image_delete(db, image_id));
    TEST_ASSERT(!pb_image_retrieve(db, image_id, &len_out));
    TEST_ASSERT(pb_errno() == PB_E_NOTFOUND);



    puts("\nDo not allow writing on version mismatch");

    pb_clear_error();
    pb_set_database_version(db, pb_get_current_version() + 1);
    TEST_ASSERT(!pb_image_delete(db, image_id));
    TEST_ASSERT(pb_errno() == PB_E_VERSION)
    pb_clear_error();
    TEST_ASSERT(!pb_image_save(db, 0, 0, &helloworld, len))
    TEST_ASSERT(pb_errno() == PB_E_VERSION)

    pb_clear_error();
    pb_set_database_version(db, pb_get_current_version() - 1);
    TEST_ASSERT(!pb_image_delete(db, image_id));
    TEST_ASSERT(pb_errno() == PB_E_VERSION)
    pb_clear_error();
    TEST_ASSERT(!pb_image_save(db, 0, 0, &helloworld, len))
    TEST_ASSERT(pb_errno() == PB_E_VERSION)
    pb_clear_error();

    /* Clean up and finish */
    pb_close_database(db);
    remove(temp_filename);
    TEST_SUCCESS();
}