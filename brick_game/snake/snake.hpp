#pragma once
#include <array>

#include "brick_game/globals.h"
#include "brick_game/models/frontendInterface.h"
#include "brick_game/models/gameModel.h"
#include "brick_game/snake/frontend.hpp"

namespace s21 {

class Snake_Game : public GameModel, public SnakeFrontendData {
 private:
  int lenght;
  std::list<Position> body;
  Direction direction;
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

  void InitBody();
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
  int START_SEG = 4;

  Snake_Game();
  Frontend_Interface* updateCurrentState(UserAction_t action) override;
  std::list<Position> GetBody() override;
  Direction GetDirection() override;
  Position GetApple() const override;
  bool IsWin() override;
  bool IsLose() override;
  void Save_HIScore() override;
  void Get_HIScore() override;
  void InitFSM() override;
  void reset() override;
};
}  // namespace s21
