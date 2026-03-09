#pragma once
#include <iostream>

#include "../globals.h"
namespace s21 {

class GameModel {
 protected:
  long timer;

 public:
  virtual GameModel* updateCurrentState(UserAction_t) = 0;
  long GetTimer() { return this->timer; };
  virtual ~GameModel() = default;
};
}  // namespace s21
