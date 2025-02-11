#include <functional>
#include <random>

using Page = std::array<unsigned int, 1000>;
bool isPrime(unsigned int number);
Page createNumbers();
int countPrimes(const Page &page);
