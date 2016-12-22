/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_MATERIAL_ITEM
#define PB_MATERIAL_ITEM

#include "query.h"

#define PB_MAT_ITEM_VAR_ID 1
#define PB_MAT_ITEM_VAR_NAME 2
#define PB_MAT_ITEM_VAR_ARTICLE_NO 3
#define PB_MAT_ITEM_VAR_N_STOCK 4
#define PB_MAT_ITEM_VAR_IMAGE_ID 5

/*
 * This structure can be extended in further releases, always keeping binary compatibility.
 */
typedef struct pb_material_item {
    int id;
    char name[65], article_no[33];
    int n_stock, image_id;
} pb_material_item;

/**
 * Retrieve a single material item from the database.
 * Convenience function that spares you from having to build your own custom query for just retrieving
 * a single material item.
 * @return NULL on error (and pb_errno will be set) or when empty, otherwise a newly allocated pb_material_item
 * that has to be freed again using pb_mat_item_free().
 */
pb_material_item * pb_mat_item_retrieve(pb_database * db, int id);

/**
 * Parses a query result row and creates a new material item from it.
 * @param query The query object, stepped to the result row.
 * @param column_mapping An array of integers assigning a variable its column index terminated with a zero.
 *  Example: { PB_MAT_ITEM_VAR_ID, 2, PB_MAT_ITEM_VAR_NAME, 3, 0 }
 * @return NULL on error (and pb_errno() is set), otherwise a pointer to a new pb_material_item.
 *  Always remember to call pb_mat_item_free() on that item again when you're done using it!
 */
pb_material_item * pb_mat_item_from_query(pb_query * query, const int * column_mapping);

/**
 * Cleanup for a material item obtained through this API.
 */
void pb_mat_item_free(pb_material_item * item);

/**
 * Update or create a material item, persistent in the database.
 * This call can be used from within a transaction.
 * @param item Points to an item structure that will be copied to persistent storage. You can optionally
 * leave item->id set at zero when creating a new material item to auto-assign a new ID.
 * @param update If this is set to 1, an UPDATE query will be performed to overwrite an existing material item.
 *  Otherwise this function will fail when encountering an existing material item.
 * @return Zero on failure and pb_errno is set, otherwise the item id.
 */
int pb_mat_item_save(pb_database * db, pb_material_item * item, int update);

/**
 * Deletes the material item specified by id from the database.
 * This call can be used from within a transaction.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_mat_item_delete(pb_database * db, int id);

#endif