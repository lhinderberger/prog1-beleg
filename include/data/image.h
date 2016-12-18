/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_IMAGE
#define PB_IMAGE

#include "database.h"

/**
 * Retrieve an image binary Blob from the database.
 * @param bytes_out Outgoing. The number of bytes in the result.
 * @return NULL on error (and pb_errno will be set) or when empty, otherwise a newly allocated array of bytes that has
 *  to be freed again using pb_image_free().
 */
void * pb_image_retrieve(pb_database * db, int id, int * bytes_out);

/**
 * Update or create an image, persistent in the database.
 * This call can be used from within a transaction.
 * @param id Set to 0 to auto-assign an image id, otherwise the preferred image id (or the id of the image to be
 *  updated, if update == 1)
 * @param update If this is set to 1, an UPDATE query will be performed to overwrite an existing image.
 *  Otherwise this function will fail when encountering an existing image.
 * @param data The image binary blob
 * @param data_bytes The length of the binary blob in bytes.
 * @return Zero on failure and pb_errno is set, otherwise the image id.
 */
int pb_image_save(pb_database * db, int id, int update, void * data, int data_bytes);

/**
 * Deletes the image specified by id from the database.
 * This call can be used from within a transaction.
 * @return 0 on success, otherwise pb_errno().
 */
int pb_image_delete(pb_database * db, int id);

/**
 * Frees an image binary blob previously obtained via pb_image_retrieve()
 * @param blob
 */
void pb_image_free(void * blob);

#endif