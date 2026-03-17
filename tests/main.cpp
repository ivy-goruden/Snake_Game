#include <gtest/gtest.h>

#include "../brick_game/globals.h"
int main(int argc, char** argv) {
  init_log();
  write_log("doing tests");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}