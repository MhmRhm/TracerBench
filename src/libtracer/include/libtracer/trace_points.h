#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER demo_app

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "libtracer/trace_points.h"

#if !defined(_TRACE_POINTS_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _TRACE_POINTS_H

#include <lttng/tracepoint.h>

// cppcheck-suppress unknownMacro
LTTNG_UST_TRACEPOINT_EVENT(demo_app, function_entry_tracepoint,
                           LTTNG_UST_TP_ARGS(const char *, function_name),
                           LTTNG_UST_TP_FIELDS(lttng_ust_field_string(
                               function_name_field, function_name)))

LTTNG_UST_TRACEPOINT_EVENT(demo_app, function_exit_tracepoint,
                           LTTNG_UST_TP_ARGS(const char *, function_name),
                           LTTNG_UST_TP_FIELDS(lttng_ust_field_string(
                               function_name_field, function_name)))

LTTNG_UST_TRACEPOINT_EVENT(
    demo_app, prime_number_tracepoint,
    LTTNG_UST_TP_ARGS(unsigned int, number, bool, is_prime),
    LTTNG_UST_TP_FIELDS(
        lttng_ust_field_integer(unsigned int, number_field, number)
            lttng_ust_field_integer(bool, is_prime_field, is_prime)))

#endif /* _TRACE_POINTS_H */

#include <lttng/tracepoint-event.h>
