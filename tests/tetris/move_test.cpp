#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisTest, MoveLeft) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  game.Falling_Handler(UserAction_t::Left);
  EXPECT_EQ(game.GetFigure().x, 4);
}

TEST(TetrisTest, MoveRight) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  game.Falling_Handler(UserAction_t::Right);
  EXPECT_EQ(game.GetFigure().x, 6);
}

TEST(TetrisTest, MoveDownTick) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  game.Falling_Handler(UserAction_t::Tick);
  EXPECT_EQ(game.GetFigure().y, 6);
}

TEST(TetrisTest, MoveDownAccelerate) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);
  game.Falling_Handler(UserAction_t::Down);
  EXPECT_EQ(game.GetFigure().y, 19);
}
