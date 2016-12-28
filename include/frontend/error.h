/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_FRONTEND_ERROR
#define PB_FRONTEND_ERROR

/**
 * Show an error message dialogue to the user and abort
 * the program.
 */
void fatal_error(const char * message);

/**
 * Retrieves an error message from the core library, displays it to
 * the user and exits the application.
 */
void fatal_pb_error();

#endif