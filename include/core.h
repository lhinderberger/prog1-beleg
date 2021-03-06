/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_CORE
#define PB_CORE

/**
 * Initializes the prog1-beleg Core (short: pb-core) library.
 * After calling this, pb_errno() and pb_error_str() are guaranteed to function, even on init failure.
 * @return 0 on success, otherwise pb_errno()
 */
int pb_init();

#endif