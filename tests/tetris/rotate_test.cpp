#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisTest, RotateI) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  // I shape: {{1, 1, 1, 1}} (1x4)
  fig.figure = {{1, 1, 1, 1}};
  game.SetFigure(fig);

  game.RotateFigure();

  // Should become 4x1
  // {{1}, {1}, {1}, {1}}
  auto rotated = game.GetFigure().figure;
  EXPECT_EQ(rotated.size(), 4);
  EXPECT_EQ(rotated[0].size(), 1);
}

TEST(TetrisTest, RotateO) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  // O shape: {{1, 1}, {1, 1}}
  fig.figure = {{1, 1}, {1, 1}};
  game.SetFigure(fig);

  game.RotateFigure();

  auto rotated = game.GetFigure().figure;
  EXPECT_EQ(rotated.size(), 2);
  EXPECT_EQ(rotated[0].size(), 2);
}
