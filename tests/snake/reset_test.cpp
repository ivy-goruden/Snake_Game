#include <gtest/gtest.h>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, ResetGame) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.NewApple();
  snake.reset();

  EXPECT_EQ(snake.Is_AteApple(), false);
  EXPECT_EQ(snake.GetLenght(), 4);
  EXPECT_EQ(snake.GetDirection(), RIGHT);
  EXPECT_EQ(snake.GetLevel(), 0);
  EXPECT_EQ(snake.GetScore(), 0);
}
