/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#include <string.h>

#include "data/database.h"
#include "data/material_item.h"
#include "logic/items_query.h"
#include "../test_utils.h"

#define N_ITEMS 5
#define BUFFER_SIZE 10

pb_material_item buffer[BUFFER_SIZE];

void clear_buffer() {
    memset(buffer, 0, BUFFER_SIZE * sizeof(pb_material_item));
}

void assert_sorted_by_art_no(int n) {
    const char * previous = NULL;
    for (int i = 0; i < n; i++) {
        if (previous)
            TEST_ASSERT(strcmp(buffer[i].article_no, previous) >= 0);
        previous = buffer[i].article_no;
    }
}

struct test_data {
    int id;
    const char * name;
    const char * article_no;
};

int main() {
    TEST_INIT();

    puts("Create database");
    const char * temp_filename = get_temp_filename();
    pb_database * db = pb_open_database(temp_filename, 1, 0);
    if (!db)
        EXIT_PB_ERRNO();

    puts("Create material items");
    struct test_data samples[] = {
        {1, "Sonic Screwdriver", "uvw-xyz-1" },
        {5, "Tardis", "uvw-xyz-1" },
        {8, "River Thames", "asdf-23" },
        {11, "ARD", "fdsa-43" },
        {13, "Driver Installer", "qwertz-789" },
    };

    pb_material_item item;
    for (int i = 0; i < N_ITEMS; i++) {
        init_example_item(&item);

        item.id = samples[i].id;
        strcpy(item.name, samples[i].name);
        strcpy(item.article_no, samples[i].article_no);

        item.id = pb_mat_item_save(db, &item, 0);
        TEST_ASSERT(item.id == samples[i].id);
    }


    puts("Do sorted list query");
    clear_buffer();
    TEST_ASSERT(pb_list_mat_items(db, buffer, PB_MAT_ITEM_VAR_ARTICLE_NO, 1, 0, BUFFER_SIZE) == N_ITEMS);
    assert_sorted_by_art_no(N_ITEMS);

    puts("Do search query");
    clear_buffer();
    TEST_ASSERT(pb_find_mat_items(db, buffer, "ard", PB_MAT_ITEM_VAR_NAME, 0, BUFFER_SIZE) == 2);
    TEST_ASSERT(pb_find_mat_items(db, buffer, "river", PB_MAT_ITEM_VAR_NAME, 0, BUFFER_SIZE) == 3);
    TEST_ASSERT(pb_find_mat_items(db, buffer, "ard", PB_MAT_ITEM_VAR_ARTICLE_NO, 0, BUFFER_SIZE) == 0);
    TEST_ASSERT(pb_find_mat_items(db, buffer, "river", PB_MAT_ITEM_VAR_ARTICLE_NO, 0, BUFFER_SIZE) == 0);
    TEST_ASSERT(pb_find_mat_items(db, buffer, "asdf", PB_MAT_ITEM_VAR_ARTICLE_NO, 0, BUFFER_SIZE) == 1);
    TEST_ASSERT(pb_find_mat_items(db, buffer, "uvw", PB_MAT_ITEM_VAR_ARTICLE_NO, 0, BUFFER_SIZE) == 2);

    /* Clean up and finish */
    pb_close_database(db);
    remove(temp_filename);
    TEST_SUCCESS();
}