/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_LIST
#define PB_LIST

/*
 * A list datatype for use in upcoming releases of the software.
 * (And for demonstration that even though there is no list data structure
 * in the entire program, this because I _cannot_ program lists and thus avoided them,
 * but rather because I chose solutions that simply didn't require lists.)
 */

typedef struct pb_list {
    struct pb_list * next;
    void * data;
} pb_list;

/**
 * Pushes a new, empty list item at the front of a given list, or creates a new list.
 * @param list List to prepend the new item to or NULL to create a new list.
 * @return The empty, new list item. Note: You must free this again using pb_list_pop!
 *  Returns NULL on error, and pb_errno will be set.
 */
pb_list * pb_list_push(pb_list * list);

/**
 * Removes the list item from the beginning of the list and deallocates it.
 * @param list
 * @return The new first list element, or NULL if there were no more elements in the list.
 *  Returns NULL on error, and pb_errno will be set.
 */
pb_list * pb_list_pop(pb_list * list);

#endif