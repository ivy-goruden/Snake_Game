#include <gtest/gtest.h>

#include "tetris_test.hpp"

// TEST(TetrisTest, LockFigure) {
//   s21::Tetris_Test game;
//   s21::figure_t fig;
//   fig.x = 5;
//   fig.y = 19;
//   fig.figure = {{1}};
//   game.SetFigure(fig);

//   game.Lock_Handler(UserAction_t::No_Action);

//   // Check if fixed board has the block
//   auto fixed = game.GetFixed();
//   EXPECT_EQ(fixed[19][5], 1);
// }

TEST(TetrisTest, LineClear) {
  s21::Tetris_Test game;
  auto fixed = game.GetFixed();
  for (int x = 0; x < 10; ++x) {
    fixed[19][x] = 1;
  }
  game.SetFixed(fixed);
  game.ValidateFixed();

  // Line 19 should be cleared (all 0)
  fixed = game.GetFixed();
  for (int x = 0; x < 10; ++x) {
    EXPECT_EQ(fixed[19][x], 0);
  }

  // Score should increase
  EXPECT_EQ(game.GetScore(), 100);  // 1 line = 100 points
}

TEST(TetrisTest, ScoreUpdate) {
  s21::Tetris_Test game;
  game.SetScore(0);
  game.UpdateScore(1);
  EXPECT_EQ(game.GetScore(), 100);

  game.SetScore(0);
  game.UpdateScore(2);
  EXPECT_EQ(game.GetScore(), 300);

  game.SetScore(0);
  game.UpdateScore(3);
  EXPECT_EQ(game.GetScore(), 700);

  game.SetScore(0);
  game.UpdateScore(4);
  EXPECT_EQ(game.GetScore(), 1500);
}
