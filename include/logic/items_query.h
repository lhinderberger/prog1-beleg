/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#ifndef PB_ITEMS_QUERY
#define PB_ITEMS_QUERY

#include "../data/database.h"
#include "../data/material_item.h"

/**
 * Find material items by a given search string
 * @param arr_out Points to a buffer array of pb_material_item instances where the query results will be written to.
 * @param search_string The search string to be matched. Matching method is implementation dependant.
 * @param field The PB_MAT_ITEM_VAR_* index of the field to be searched. Currently only supports NAME and ARTICLE_NO
 * @param offset SQL request offset - The number of items to skip before returning results.
 * @param limit The maximum number of items to be read (usually the size of arr_out).
 * @return The number of items read, or a negative indicator on error, plus pb_errno will be set.
 */
int pb_find_mat_items(pb_database * db, pb_material_item * arr_out, const char * search_string, int field, int offset, int limit);

/**
 * List material items, optionally sorted by a certain field.
 * @param arr_out Points to a buffer array of pb_material_item instances where the query results will be written to
 * @param sort_field The PB_MAT_ITEM_VAR_* index of the field to sort the list by. If zero, the list sorting (if any)
 *  will be implementation defined
 * @param sort_ascending If this is set to 0, sorting will be descending, otherwise ascending.
 * @param offset SQL request offset - The number of items to skip before returning results.
 * @param limit The maximum number of items to be read (usually the size of arr_out).
 * @return The number of items read, or a negative indicator on error, plus pb_errno will be set.
 */
int pb_list_mat_items(pb_database * db, pb_material_item * arr_out, int sort_field, int sort_ascending, int offset, int limit);

#endif