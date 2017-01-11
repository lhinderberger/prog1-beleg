/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_FRONTEND_LIST
#define PB_FRONTEND_LIST

#include "data/material_item.h"

/**
 * Clears the material list view.
 */
void clear_list();

/**
 * Appends the material item given to the bottom of the list
 */
void render_material_item(pb_material_item * item);

/**
 * Brings up the material list screen
 */
void show_material_list();

#endif