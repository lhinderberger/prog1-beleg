/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_ERROR_PRIVATE
#define PB_ERROR_PRIVATE

#include "error.h"

/**
 * Sets the error state.
 * Note: This will set the custom error string to a generic error string if pb_errno is PB_ERR_CUSTOM because
 * you are supposed to use pb_custom_error for custom errors.
 * @param pb_errno The error type to be written to pb_errno.
 */
void pb_error(int pb_errno);

/**
 * Sets the error state to a custom error and sets a custom error string.
 * Exclusively use this function instead of pb_error(PB_ERR_CUSTOM) to trigger custom errors!
 */
void pb_custom_error(const char * err_string);

#endif