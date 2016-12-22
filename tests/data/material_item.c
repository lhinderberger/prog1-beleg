/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <memory.h>
#include "../test_utils.h"

#include "data/database.h"
#include "data/material_item.h"

void init_example_item(pb_material_item * item) {
    memset(item, 0, sizeof(pb_material_item));
    strcpy(item->name, "Sonic Screwdriver");
    strcpy(item->article_no, "DR-19WHO63");
    item->n_stock = 234;
}

int main() {
    TEST_INIT();

    puts("Create database");
    const char * temp_filename = get_temp_filename();
    pb_database * db = pb_open_database(temp_filename, 1, 0);
    if (!db)
        EXIT_PB_ERRNO();

    puts("Create material item");
    pb_material_item item;
    init_example_item(&item);
    item.id = pb_mat_item_save(db, &item, 0);
    TEST_ASSERT(item.id != 0);

    puts("Test retrieval (and thus construction from query result)");
    pb_material_item * item_out = pb_mat_item_retrieve(db, item.id);
    TEST_ASSERT(item_out != NULL);
    /* Every structure created by our library shall be sanitized using memset(),
     * otherwise the test shall fail.
     */
    TEST_ASSERT(!memcmp(item_out, &item, sizeof(pb_material_item)));
    pb_mat_item_free(item_out);

    puts("Test material item deletion");
    TEST_ASSERT(!pb_mat_item_delete(db, item.id));

    puts("Creating with unterminated strings should fail");
    init_example_item(&item);
    for (int i = 0; i < 65; i++)
        item.name[i] = 'a';
    TEST_ASSERT(!pb_mat_item_save(db, &item, 0) && pb_errno() == PB_E_TOOLONG);

    init_example_item(&item);
    for (int i = 0; i < 33; i++)
        item.article_no[i] = 'a';
    TEST_ASSERT(!pb_mat_item_save(db, &item, 0) && pb_errno() == PB_E_TOOLONG);

    puts("Creating with negative n_stock should fail");
    init_example_item(&item);
    item.n_stock = -1;
    TEST_ASSERT(!pb_mat_item_save(db, &item, 0) && pb_errno() == PB_E_RANGE);

    /* Clean up and finish */
    pb_close_database(db);
    remove(temp_filename);
    TEST_SUCCESS();
}