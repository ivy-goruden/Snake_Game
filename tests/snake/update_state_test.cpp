#include <gtest/gtest.h>

#include "snake_test.hpp"

TEST(SnakeUpdateState, InitialState) {
  s21::Snake_Test game;
  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_MOVE);
}

TEST(SnakeUpdateState, MoveUpdate) {
  s21::Snake_Test game;
  Position initialHead = game.GetBody().front();

  // Update state with Tick (Forward)
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_MOVE);
  Position newHead = game.GetBody().front();
  EXPECT_NE(initialHead, newHead);
}

TEST(SnakeUpdateState, EatingTransition) {
  s21::Snake_Test game;

  // Setup snake to be one step away from apple
  // Let's force apple position and snake position
  Position head = game.GetBody().front();
  // Assume moving RIGHT by default
  Position applePos = {head.x + 1, head.y};
  game.SetApple(applePos);

  // Update state - should move head to apple and trigger transition to EATING
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_EATING);

  // Next update should handle eating and return to MOVE
  game.updateCurrentState(Tick);
  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_MOVE);

  // Check score increased (default score is 0)
  EXPECT_GT(game.GetScore(), 0);

  // Check length increased (start seg is 4)
  EXPECT_GT(game.GetBody().size(), 4);
}

TEST(SnakeUpdateState, GameOverCollisionWall) {
  s21::Snake_Test game;

  // Move to right wall
  // Width is 10. Head at start is usually center (5, 9)
  // Let's just manually set body to edge
  std::list<Position> body;
  body.push_back({9, 5});  // At right edge
  body.push_back({8, 5});
  game.SetBody(body);
  game.SetDirection(RIGHT);

  // Move Right - should hit wall (x=10 is invalid)
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_OVER);
  EXPECT_TRUE(game.IsLose());
}

TEST(SnakeUpdateState, GameOverCollisionSelf) {
  s21::Snake_Test game;

  // Setup self collision scenario
  // Head at (5,5), body at (6,5), (6,6), (5,6), (5,5)...
  //     H->(5,5)
  //     |
  //    (5,6)-(6,6)
  //           |
  //          (6,5)
  // Actually simpler:
  // O-O-O
  // |   |
  // O---H < move left into body

  // Let's use a simple setup where next move hits body
  std::list<Position> body;
  body.push_back({5, 5});  // Head
  body.push_back({4, 5});
  body.push_back({4, 6});
  body.push_back({5, 6});  // Body part
  body.push_back({5, 7});

  game.SetBody(body);
  game.SetDirection(DOWN);

  // Move Down -> (5,6) which is occupied
  game.updateCurrentState(Tick);

  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_OVER);
}

TEST(SnakeUpdateState, ResetFromOver) {
  s21::Snake_Test game;

  // Force state to OVER
  game.SetState(s21::Snake_Test::State::ST_OVER);

  // Press Enter to reset
  game.updateCurrentState(Enter);

  EXPECT_EQ(game.GetState(), s21::Snake_Test::State::ST_MOVE);
  EXPECT_EQ(game.GetScore(), 0);
}

TEST(SnakeUpdateState, InterfacePolymorphism) {
  s21::Snake_Test game;
  s21::GameModel* model = &game;

  // Use GameModel interface
  s21::Frontend_Interface* frontend = model->updateCurrentState(Tick);

  // Verify returned interface is valid and points to the game object
  EXPECT_EQ(frontend->GetScore(), 0);
  EXPECT_FALSE(frontend->IsWin());
  EXPECT_FALSE(frontend->IsLose());
}
