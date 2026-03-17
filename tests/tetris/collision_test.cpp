#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisTest, NoCollision) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  // Check collision at current position + 1 (down)
  // 5, 6 should be free
  EXPECT_FALSE(game.collision(fig.x, fig.y + 1, fig.figure));
}

TEST(TetrisTest, CollisionWallLeft) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 0;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  // Move left to -1
  EXPECT_TRUE(game.collision(fig.x - 1, fig.y, fig.figure));
}

TEST(TetrisTest, CollisionWallRight) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 9;  // WIDTH is 10, index 9 is last valid
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  // Move right to 10
  EXPECT_TRUE(game.collision(fig.x + 1, fig.y, fig.figure));
}

TEST(TetrisTest, CollisionFloor) {
  s21::Tetris_Test game;
  s21::figure_t fig;
  fig.x = 5;
  fig.y = 19;  // HEIGHT is 20, index 19 is last valid
  fig.figure = {{1}};
  game.SetFigure(fig);

  // Move down to 20
  EXPECT_TRUE(game.collision(fig.x, fig.y + 1, fig.figure));
}

TEST(TetrisTest, CollisionFixed) {
  s21::Tetris_Test game;

  // Set a fixed block at 5, 6
  auto fixed = game.GetFixed();
  fixed[6][5] = 1;
  game.SetFixed(fixed);

  s21::figure_t fig;
  fig.x = 5;
  fig.y = 5;
  fig.figure = {{1}};
  game.SetFigure(fig);

  // Move down to 5, 6 which is occupied
  EXPECT_TRUE(game.collision(fig.x, fig.y + 1, fig.figure));
}
