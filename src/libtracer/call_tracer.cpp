#include "libtracer/call_tracer.h"
#include "libtracer/trace_points.h"

CallTracer::CallTracer(const char *name) : functionName{name} {
  lttng_ust_tracepoint(demo_app, function_entry_tracepoint, functionName);
}
CallTracer::~CallTracer() {
  lttng_ust_tracepoint(demo_app, function_exit_tracepoint, functionName);
}