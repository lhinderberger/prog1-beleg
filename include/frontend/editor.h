/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_FRONTEND_EDITOR
#define PB_FRONTEND_EDITOR

#include "data/material_item.h"

/**
 * Opens up a new editor for creating a new item or editing an existing one.
 * @param create Zero for editing an existing item, otherwise creating a new one.
 * @param item An allocated item object pointing to an existing item when create is 1, otherwise ignored.
 */
void open_editor(pb_material_item * item, int create);

#endif