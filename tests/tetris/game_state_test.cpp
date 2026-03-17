#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisTest, GameOver) {
  s21::Tetris_Test game;

  // Simulate overflow
  auto fixed = game.GetFixed();
  fixed[0][5] = 1;  // Block at top row
  game.SetFixed(fixed);

  EXPECT_TRUE(game.isOverflowed());
  EXPECT_TRUE(game.isOver());
}

TEST(TetrisTest, Win) {
  s21::Tetris_Test game;
  game.SetScore(6000);  // > 5000 is win condition in tetris.cpp
  EXPECT_TRUE(game.isWin());
  EXPECT_TRUE(game.isOver());
}

TEST(TetrisTest, Reset) {
  s21::Tetris_Test game;
  game.SetScore(1000);

  game.Over_Handler(UserAction_t::Enter);

  EXPECT_EQ(game.GetScore(), 0);
  // Check board cleared
  auto fixed = game.GetFixed();
  EXPECT_EQ(fixed[0][0], 0);
}
