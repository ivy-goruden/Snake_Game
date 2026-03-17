#include <gtest/gtest.h>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, RightMove) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(Direction::RIGHT);
  Position head = snake.GetBody().front();
  snake.Forward();
  EXPECT_EQ(snake.GetBody().front(), (Position{int(head.x) + 1, int(head.y)}));
}
// Test case 3
TEST(Snake_Test, UpMove) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(Direction::UP);
  Position head = snake.GetBody().front();
  snake.Forward();
  EXPECT_EQ(snake.GetBody().front(), (Position{int(head.x), int(head.y - 1)}));
}
// Test case 4
TEST(Snake_Test, LeftMove) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(Direction::LEFT);
  Position head = snake.GetBody().front();
  snake.Forward();
  EXPECT_EQ(snake.GetBody().front(), (Position{int(head.x) - 1, int(head.y)}));
}
// Test case 5
TEST(Snake_Test, DownMove) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetDirection(Direction::DOWN);
  Position head = snake.GetBody().front();
  snake.Forward();
  EXPECT_EQ(snake.GetBody().front(), (Position{int(head.x), int(head.y + 1)}));
}
