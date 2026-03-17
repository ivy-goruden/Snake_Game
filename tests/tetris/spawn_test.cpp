#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisTest, NewFigure) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  game.New_Figure(fig);
  EXPECT_FALSE(fig.figure.empty());
  EXPECT_GE(fig.x, 0);
  EXPECT_GE(fig.y, 0);
}