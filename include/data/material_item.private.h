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

#endif