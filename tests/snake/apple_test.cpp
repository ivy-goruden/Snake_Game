#include <gtest/gtest.h>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, NewApple) {
  s21::Snake_Test snake = s21::Snake_Test();
  auto body = snake.GetBody();
  snake.SetApple(snake.GetBody().front());
  snake.NewApple();
  // std::list<Position>
  bool overlaps = false;
  Position apple = snake.GetApple();
  for (auto i = body.begin(); i != body.end(); i++) {
    if (*i == apple) {
      overlaps = true;
      break;
    }
  }
  Position head = *(body.begin());
  EXPECT_EQ(overlaps, false);
  EXPECT_EQ((head == apple), false);
}
