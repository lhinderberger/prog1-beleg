#ifndef TEST_UTILS
#define TEST_UTILS

#define EXIT_PB_ERRNO() exit_pb_error(pb_errno())

void exit_pb_error(int pb_errno);

#endif