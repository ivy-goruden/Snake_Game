#pragma once
#include <iostream>

#include "../globals.h"
#include "frontendInterface.h"
namespace s21 {

class GameModel {
 protected:
  long timer;

 public:
  virtual Frontend_Interface* updateCurrentState(UserAction_t) = 0;
  long GetTimer() { return this->timer; };
  bool Yes() { return true; }
  bool No() { return false; }
  virtual void InitFSM() = 0;
  virtual void reset() = 0;
  virtual ~GameModel() = default;
};
}  // namespace s21
