#pragma once
#include <array>
#include <list>

#include "../globals.h"
#include "../models/gameModel.h"

namespace s21 {

template <size_t width, size_t height>
class Snake_Game : public GameModel {
 private:
  int lenght;
  int WIDTH = width;
  int HEIGHT = height;
  std::list<Position> body;
  int direction;
  Position apple;
  bool ateApple;

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

  Snake_Game();
  GameModel* updateCurrentState(UserAction_t action) override;
};
}  // namespace s21