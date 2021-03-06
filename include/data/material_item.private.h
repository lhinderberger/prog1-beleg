/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_MATERIAL_ITEM_PRIVATE
#define PB_MATERIAL_ITEM_PRIVATE

#include "material_item.h"

/**
 * Allocates and initializes a new pb_material_item for internal use.
 * @return NULL on error (an pb_errno() will be set), otherwise a freshly allocated pb_material_item,
 *  which has to be freed again using pb_mat_item_free().
 */
pb_material_item * pb_mat_item_create();

/**
 * Implements pb_mat_item_from_query
 * @param item_out Points to a material item allocated from within this library where the output shall be written.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_mat_item_from_query_ptr(pb_query * query, pb_material_item * item_out, const int * column_mapping);

/**
 * Returns the SQL column name as string for the given column ID
 * @param column_id A column ID as in as in PB_MAT_ITEM_VAR_*
 * @return A constant string or NULL on error.
 */
const char * pb_mat_item_get_column_name(int column_id);

#endif