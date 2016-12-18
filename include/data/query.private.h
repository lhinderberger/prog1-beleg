/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_QUERY_PRIVATE
#define PB_QUERY_PRIVATE

#include <sqlite3.h>

#include "query.h"

struct pb_query {
    sqlite3 * connection;
    sqlite3_stmt * statement;
};

#endif