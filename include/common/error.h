/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_ERROR
#define PB_ERROR

#define PB_E_CUSTOM 951123

/**
 * Retrieves the error identifier.
 * @return Returns the error identifier number, or 0 if there was no error. You can use that error identifier
 *  to retrieve a descriptive error string via pb_error_str().
 */
int pb_errno();

/**
 * Retrieves a descriptive error string.
 * @param pb_errno An error identifier, usually previously obtained via pb_errno().
 * @return Always returns a pointer to a valid string, descripting the error condition signaled by pb_errno, or, if
 *  an invalid pb_errno was given, a generic error string. This never returns NULL.
 */
const char * pb_error_str(int pb_errno);

#endif