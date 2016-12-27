/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */

#ifndef PB_CSV_EXPORT
#define PB_CSV_EXPORT

#include "logic/items_query.h"

/**
 * Writes the material items to a csv file.
 * @param filename The filename of the csv file to produce. Provide "stdout" to write to terminal.
 * @return 0 on success, otherwise pb_errno()
 */
int pb_csv_export_mat_items(pb_material_item_buffer items, int n_items, const char * filename);

#endif