#pragma once
#include <iostream>

#include "brick_game/globals.h"
#include "brick_game/models/frontendInterface.h"
namespace s21 {

class GameModel {
 protected:
  long timer;

 public:
  int WIDTH;
  int HEIGHT;
  virtual Frontend_Interface* updateCurrentState(UserAction_t) = 0;
  long GetTimer() { return this->timer; };
  virtual ~GameModel() = default;
};
}  // namespace s21
