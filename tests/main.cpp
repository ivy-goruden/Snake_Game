#include <gtest/gtest.h>

#include "../brick_game/snake/snake.hpp"
#include "../brick_game/tetris/tetris.hpp"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}