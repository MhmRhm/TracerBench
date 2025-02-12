#include "libapp/libapp.h"
#include "libtracer/trace_points.h"
#include <math.h>

namespace {
std::random_device device{};
} // namespace

Tracer::Tracer(const char *name) : functionName{name} {
  lttng_ust_tracepoint(demo_app, function_entry_tracepoint, functionName);
}
Tracer::~Tracer() {
  lttng_ust_tracepoint(demo_app, function_exit_tracepoint, functionName);
}

bool isPrime(unsigned int number) {
  if (number <= 1)
    return false;
  if (number <= 3)
    return true;
  if (number % 2 == 0 || number % 3 == 0)
    return false;

  for (unsigned int i = 5; i * i <= number; i += 6) {
    if (number % i == 0 || number % (i + 2) == 0) {
      return false;
    }
  }
  return true;
}

Page createNumbers() {
  Tracer tracer(__func__);
  Page page{};
  for (auto itr{page.begin()}; itr < page.end(); itr += 1) {
    *itr = device();
  }
  return page;
}

int countPrimes(const Page &page) {
  Tracer tracer(__func__);
  int count{};
  for (auto itr{page.cbegin()}; itr < page.cend(); itr += 1) {
    bool prime{isPrime(*itr)};
    lttng_ust_tracepoint(demo_app, prime_number_tracepoint, *itr, prime);
    if (prime) {
      prime = true;
      count += 1;
    }
  }
  return count;
}
