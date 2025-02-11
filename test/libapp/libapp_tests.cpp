#include "libapp/libapp.h"
#include "gtest/gtest.h"

TEST(IsPrimeTests, IsPrime23) {
  // given
  unsigned int number{23};

  // when
  bool is_prime{isPrime(number)};

  // then
  ASSERT_TRUE(is_prime);
}

TEST(IsPrimeTests, IsPrime42) {
  // given
  unsigned int number{42};

  // when
  bool is_prime{isPrime(number)};

  // then
  ASSERT_FALSE(is_prime);
}
