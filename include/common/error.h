/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_ERROR
#define PB_ERROR

#define PB_E_NOT_IMPLEMENTED    -1
#define PB_E_DB_BACKEND         -2
#define PB_E_VERSION            -3
#define PB_E_EXISTS             -4
#define PB_E_MALLOC             -5
#define PB_E_NULLPTR            -6
#define PB_E_RANGE              -7
#define PB_E_NOTFOUND           -8
#define PB_E_TOOLONG            -9
#define PB_E_CUSTOM             951123

/**
 * Retrieves the error identifier.
 * @return Returns the error identifier number, or 0 if there was no error. You can use that error identifier
 *  to retrieve a descriptive error string via pb_error_str().
 */
int pb_errno();

/**
 * Clears pb_errno and resets the custom error string.
 */
void pb_clear_error();

/**
 * Retrieves a descriptive error string.
 * @param pb_errno An error identifier, usually previously obtained via pb_errno().
 * @return Always returns a pointer to a valid string, descripting the error condition signaled by pb_errno, or, if
 *  an invalid pb_errno was given, a generic error string. This never returns NULL.
 */
const char * pb_error_str(int pb_errno);

#endif