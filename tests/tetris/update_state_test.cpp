#include <gtest/gtest.h>

#include "tetris_test.hpp"

TEST(TetrisUpdateState, InitialState) {
  s21::Tetris_Test game;
  // Constructor calls InitFSM and sets state to SPAWN then New_Figure
  // Wait, constructor does:
  // this->cur_state = ST_SPAWN;
  // New_Figure(this->nextFigure);

  // Actually, let's check what state it ends up in.
  // Ideally it starts in SPAWN, but does it stay there?
  // No, constructor sets cur_state = ST_SPAWN.
  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_SPAWN);
}

TEST(TetrisUpdateState, SpawnToFalling) {
  s21::Tetris_Test game;
  // Start in SPAWN

  // Update should transition to FALLING
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_FALLING);
}

TEST(TetrisUpdateState, FallingMove) {
  s21::Tetris_Test game;
  // Move to FALLING
  game.updateCurrentState(Tick);
  ASSERT_EQ(game.GetState(), s21::Tetris_Test::State::ST_FALLING);

  s21::figure_t fig = game.GetFigure();
  int startX = fig.x;
  int startY = fig.y;

  // Move Right
  game.updateCurrentState(Right);
  fig = game.GetFigure();
  EXPECT_EQ(fig.x, startX + 1);
  EXPECT_EQ(fig.y, startY);

  // Move Down (Tick)
  game.updateCurrentState(Tick);
  fig = game.GetFigure();
  EXPECT_EQ(fig.x, startX + 1);
  EXPECT_EQ(fig.y, startY + 1);
}

TEST(TetrisUpdateState, LockTransition) {
  s21::Tetris_Test game;
  // Move to FALLING
  game.updateCurrentState(Tick);

  // Force figure to bottom
  s21::figure_t fig = game.GetFigure();
  // Height is 20. Figure usually spawns at top.
  // Let's set Y to 19 (bottom row is 19).
  // If figure height is 2 (block), Y=18 might be bottom.
  // Let's just put it very low.
  fig.y = 19;
  game.SetFigure(fig);

  // Update - should detect collision with floor and switch to LOCK
  // Wait, collision is checked for y+1.
  // If y=19, y+1=20 (out of bounds). Collision!
  // So trigger ST_LOCK should fire.

  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_LOCK);
}

TEST(TetrisUpdateState, LockToSpawn) {
  s21::Tetris_Test game;
  game.SetState(s21::Tetris_Test::State::ST_LOCK);

  // Update should lock the piece and transition to SPAWN (if not game over)
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_SPAWN);
}

TEST(TetrisUpdateState, LockToOver) {
  s21::Tetris_Test game;
  game.SetState(s21::Tetris_Test::State::ST_LOCK);

  // Setup condition for Game Over
  // Overflow: If we lock a piece and it's still colliding or "overflowed"?
  // isOverflowed() checks if any block in fixed is in the top row?
  // Let's check tetris.cpp implementation of isOverflowed or isOver.
  // isOver() -> isWin() || isOverflowed()
  // isOverflowed() -> usually checks if fixed blocks exist in spawn area?

  // Let's fill the board so next spawn or current lock causes overflow.
  auto fixed = game.GetFixed();
  fixed[0][0] = 1;  // Fill one cell in top row
  game.SetFixed(fixed);

  // When in LOCK state, Lock_Handler runs.
  // Then triggers check isOver().
  // isOver() checks isOverflowed().
  // If fixed[0] has blocks, isOverflowed() should be true?
  // Need to verify isOverflowed implementation.
  // Assuming it checks top row.

  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_OVER);
}

TEST(TetrisUpdateState, ResetFromOver) {
  s21::Tetris_Test game;
  game.SetState(s21::Tetris_Test::State::ST_OVER);

  game.updateCurrentState(Enter);

  EXPECT_EQ(game.GetState(), s21::Tetris_Test::State::ST_SPAWN);
  EXPECT_EQ(game.GetScore(), 0);
}

TEST(TetrisUpdateState, InterfacePolymorphism) {
  s21::Tetris_Test game;
  s21::GameModel* model = &game;

  // Use GameModel interface
  s21::Frontend_Interface* frontend = model->updateCurrentState(Tick);

  // Verify returned interface is valid and points to the game object
  EXPECT_EQ(frontend->GetScore(), 0);
  EXPECT_FALSE(frontend->IsWin());
  EXPECT_FALSE(frontend->IsLose());
}
