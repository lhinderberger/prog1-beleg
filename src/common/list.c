/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#include "common/error.private.h"
#include "common/list.h"

#include <malloc.h>
#include <memory.h>

pb_list * pb_list_push(pb_list * list) {
    pb_clear_error();

    /* Allocate a new list item */
    pb_list * new_list = (pb_list*)malloc(sizeof(pb_list));
    if (!new_list) {
        pb_error(PB_E_MALLOC);
        return NULL;
    }

    /* Initialize it */
    memset(new_list, 0, sizeof(pb_list));
    new_list->next = list;

    return new_list;
}

pb_list * pb_list_pop(pb_list * list) {
    pb_clear_error();

    if (!list)
        return NULL;

    pb_list * next = list->next;
    free(list);
    return next;
}