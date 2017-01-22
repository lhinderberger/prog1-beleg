/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include <string.h>
#include <stdio.h>

#include "../test_utils.h"
#include "common/list.h"

int main() {
    TEST_INIT();

    puts("Building and testing a list");
    pb_list * list = pb_list_push(NULL);
    TEST_ASSERT(list != NULL);
    TEST_ASSERT(list->data == NULL);
    TEST_ASSERT(list->next == NULL);

    puts("Destroying list");
    while (list) {
        pb_list * next = list->next;
        list = pb_list_pop(list);
        TEST_ASSERT(list == next);
    }
    TEST_ASSERT(pb_errno() == 0);

    TEST_SUCCESS();
}