#include <gtest/gtest.h>

#include <list>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, CheckWinInit) {
  s21::Snake_Test snake = s21::Snake_Test();
  EXPECT_EQ(snake.IsWin(), false);
}

TEST(Snake_Test, CheckWin) {
  s21::Snake_Test snake = s21::Snake_Test();
  std::list<Position> winBody = std::list<Position>(WIDTH * HEIGHT);
  for (auto i = 0; i < HEIGHT; ++i) {
    for (auto x = 0; x < WIDTH; ++x) {
      winBody.push_back(Position{i, x});
    }
  }
  snake.SetBody(winBody);
  EXPECT_EQ(snake.IsWin(), true);
}
