#include <gtest/gtest.h>

#include <list>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, CheckLose) {
  s21::Snake_Test snake = s21::Snake_Test();
  EXPECT_EQ(snake.IsLose(), false);
}

TEST(Snake_Test, CheckSelfCollision) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(LEFT);
  std::list<Position> newBody = std::list<Position>(
      {Position{0, 1}, Position{1, 1}, Position{2, 1}, Position{2, 0},
       Position{1, 0}, Position{0, 0}, Position{0, 1}, Position{0, 2}});
  snake.SetBody(newBody);
  EXPECT_EQ(snake.IsLose(), true);
}
TEST(Snake_Test, CheckBorderCollision) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(UP);
  std::list<Position> newBody({Position{0, -1}});
  snake.SetBody(newBody);
  EXPECT_EQ(snake.IsLose(), true);
}
