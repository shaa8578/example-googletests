#include <gtest/gtest.h>

#include <clocale>
#include <iostream>

//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  setlocale(LC_ALL, "Russian");

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
