#include <gtest/gtest.h>

#include <string>

//------------------------------------------------------------------------------
std::string stringFunction(int value) {
  return std::to_string(value);
}

TEST(Test_Simple, test_string) {
  const std::string expected_text = "12";
  const int test_value = 12;
  auto actual_text = stringFunction(test_value);
  ASSERT_STREQ(actual_text.c_str(), expected_text.c_str());
}

//------------------------------------------------------------------------------
double doubleFunction(int value) {
  return double(value) * 100.001;
}

TEST(Test_Simple, test_double) {
  const double expected_value = 12 * 100.001;
  const int test_value = 12;
  auto actual_value = doubleFunction(test_value);
  ASSERT_DOUBLE_EQ(actual_value, expected_value);
}
