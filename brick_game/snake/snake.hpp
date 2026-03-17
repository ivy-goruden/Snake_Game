#pragma once
#include <array>

#include "../globals.h"
#include "../models/frontendInterface.h"
#include "../models/gameModel.h"
#include "frontend.hpp"

namespace s21 {

class Snake_Game : public GameModel, public SnakeFrontendData {
 protected:
  int lenght;
  std::list<Position> body;
  Direction direction;
  Position apple = {};  // initialize x and y with default
  bool ateApple;
  int width = 10;
  int height = 20;

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

  // Handlers
  void Move_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void Eating_Handler(UserAction_t);
  // Triggers
  bool FoundApple();
  bool Smashed();
  bool isWin();
  bool isOver();
  // Helpers
  void Forward();
  void NewApple();
  void InitBody();
  void ValidateBody();
  Position getNextPos();

 public:
  int START_SEG = 4;

  Snake_Game();
  Frontend_Interface* updateCurrentState(UserAction_t action) override;
  std::list<Position> GetBody() override;
  Direction GetDirection() override;
  Position GetApple() const override;
  bool IsWin() override;
  bool IsLose() override;
  void InitFSM() override;
  void reset() override;
  void Save_HIScore() override;
  void Get_HIScore() override;
};
}  // namespace s21
