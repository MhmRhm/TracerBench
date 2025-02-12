#include "libtracer/trace_points.h"
#include <functional>
#include <random>

class Tracer {
public:
  Tracer(const char *name);
  ~Tracer();

private:
  const char *functionName;
};
using Page = std::array<unsigned int, 1000>;
bool isPrime(unsigned int number);
Page createNumbers();
int countPrimes(const Page &page);
