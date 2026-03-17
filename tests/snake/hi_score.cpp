#include <gtest/gtest.h>

#include "../../brick_game/globals.h"
#include "snake_test.hpp"

// Test case 1
TEST(Snake_Test, Get_Score) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetHIScore(0);
  snake.SetScore(500);
  int hi_score = snake.GetHighScore();
  EXPECT_EQ(hi_score, 500);
}

TEST(Snake_Test, Restore_HIScore) {
  s21::Snake_Test snake = s21::Snake_Test();
  snake.SetHIScore(200);
  snake.Save_HIScore();
  snake.SetHIScore(0);
  snake.Get_HIScore();
  int hi_score = snake.GetHighScore();
  EXPECT_EQ(hi_score, 200);
}
