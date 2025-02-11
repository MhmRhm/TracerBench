#include "libapp/libapp.h"
#include "libtracer/trace_points.h"
#include <math.h>

namespace {
std::random_device device{};
} // namespace

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
  lttng_ust_tracepoint(demo_app, function_call_tracepoint, __func__);
  Page page{};
  for (auto itr{page.begin()}; itr < page.end(); itr += 1) {
    *itr = device();
  }
  return page;
}

int countPrimes(const Page &page) {
  lttng_ust_tracepoint(demo_app, function_call_tracepoint, __func__);
  int count{};
  for (auto itr{page.cbegin()}; itr < page.cend(); itr += 1)
    if (isPrime(*itr)) {
      lttng_ust_tracepoint(demo_app, prime_number_tracepoint, *itr);
      // std::cerr << *itr << std::endl;
      count += 1;
    }
  return count;
}
