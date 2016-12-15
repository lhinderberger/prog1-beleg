/* This file is part of prog1-beleg
 * Written by Lucas Hinderberger, HTW Dresden, winter term of 2016/17
 * See README for details
 */
#ifndef PB_QUERY
#define PB_QUERY

#include "database.h"

typedef pb_query*;

/* In the standard implementation, this interface provides a thin wrapper around SQLite's
 * prepared statements, hiding all technical details of the database implementation while
 * still providing near-full access to its functionality (leaving out any functionality that
 * doesn't make sense in the context of this application).
 * This is a clean and very scalable solution, because the database backend
 * could be swapped out at very low cost if the application ever grows (theoretically).
 */

/**
 * Creates a prepared statement type query.
 * @param db The database to perform the query on.
 * @param sql A SQL query (refer to SQLite documentation for format)
 * @param sql_bytes If this is negative, all of sql until a NULL terminator will be read. Otherwise
 *  sql_max_length bytes of sql will be read.
 * @return NULL on error (and pb_errno will be set) or a pointer to a newly created pb_query object.
 *  Always remember to discard that object again after use, using pb_query_discard().
 */
pb_query * pb_query_prepare(pb_database * db, const char * sql, int sql_bytes);

/**
 * Binds an Integer value to a parameter.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_query_bind_int(pb_query * query, int param_index, int value);

/**
 * Binds the NULL value to a parameter.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_query_bind_null(pb_query * query, int param_index);

/**
 * Binds a String value to a parameter.
 * @param value_bytes If this is negative, all of value until a NULL terminator will be read. Otherwise value_bytes
 *  bytes of value will be read.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_query_bind_string(pb_query * query, int param_index, const char * value, int value_bytes);

/**
 * Retrieve the next data row from query.
 * Use pb_query_column_* functions to retrieve column values.
 * @return Zero on success, otherwise pb_errno().
 */
int pb_query_step(pb_query * query);

/**
 * Retrieve a Blob value from the current result row.
 * @param bytes_out Outgoing. The number of bytes in the result.
 * @return NULL on error (and pb_errno will be set) or when empty, otherwise an array of bytes, which has to be
 *  instantly evaluated, as it will be freed or overwritten by subsequent calls to this API.
 */
const void * pb_query_column_blob(pb_query * query, int column_index, int * bytes_out);

/**
 * Retrieve a Integer value from the current result row.
 * @return Returns the Integer value read at the current row/column.
 *  When an error occurs, pb_errno will be set and the function is guaranteed to return 0 in that case.
 */
const void * pb_query_column_int(pb_query * query, int column_index, int * bytes_out);

/**
 * Retrieve a String value from the current result row.
 * @param bytes_out Outgoing. The number of bytes in the result.
 * @return NULL on error (and pb_errno will be set) or when empty, otherwise an array of characters, which has to be
 *  instantly evaluated, as it will be freed or overwritten by subsequent calls to this API.
 */
const char * pb_query_column_string(pb_query * query, int column_index, int * bytes_out);

/**
 * Resets a query so that it can be executed again.
 * Note: This does not reset the parameter bindings!
 * @return Zero on success, otherwise pb_errno().
 */
int pb_query_reset(pb_query * query);

/**
 * Discards a pb_query.
 * Always remember to call this after the use of your pb_query object has ended!
 * @param query
 */
void pb_query_discard(pb_query * query);

#endif