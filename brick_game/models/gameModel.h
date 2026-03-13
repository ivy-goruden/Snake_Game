#pragma once
#include <iostream>

#include "brick_game/globals.h"
#include "brick_game/models/frontendInterface.h"
namespace s21 {

class GameModel {
 protected:
  long timer;

 public:
  virtual Frontend_Interface* updateCurrentState(UserAction_t) = 0;
  long GetTimer() { return this->timer; };
  virtual void Save_HIScore() = 0;
  virtual void Get_HIScore() = 0;
  virtual void InitFSM() = 0;
  virtual void reset() = 0;
  virtual ~GameModel() = default;
};
}  // namespace s21
