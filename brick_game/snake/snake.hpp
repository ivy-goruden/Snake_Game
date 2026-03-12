#pragma once
#include <array>
#include <list>

#include "brick_game/globals.h"
#include "brick_game/models/frontendInterface.h"
#include "brick_game/models/gameModel.h"
#include "brick_game/snake/frontend.hpp"

namespace s21 {

class Snake_Game : public GameModel, public SnakeFrontendData {
 private:
  int lenght;
  std::list<Position> body;
  int direction;
  Position apple;
  bool ateApple;
  int score = 0;
  int highScore = 0;
  int level = 0;

  typedef enum {
    ST_MOVE,
    ST_OVER,
    ST_EATING,
    SIZE,
  } State;

  typedef bool (Snake_Game::*Trigger)();
  typedef void (Snake_Game::*Handler)(UserAction_t);

  std::array<std::array<Trigger, SIZE>, SIZE> FSM_Triggers;
  std::array<Handler, SIZE> FSM_Handlers;
  State cur_state;

  void Move_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void Eating_Handler(UserAction_t);
  bool FoundApple();
  bool Smashed();
  bool isWin();
  bool isOver();
  void Forward();
  void NewApple();
  Position getNextPos();
  bool Yes();
  bool No();

 public:
  static const int LEFT = 0;
  static const int UP = 1;
  static const int RIGHT = 2;
  static const int DOWN = 3;
  int width = WIDTH;
  int height = HEIGHT;

  Snake_Game();
  Frontend_Interface* updateCurrentState(UserAction_t action) override;
  std::list<Position> GetBody() override;
  int Width() const override;
  int Height() const override;
  int GetDirection() override;
  int GetScore() const override;
  int GetHighScore() const override;
  int GetLevel() const override;
};
}  // namespace s21
