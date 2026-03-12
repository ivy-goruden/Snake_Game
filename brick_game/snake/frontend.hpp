#pragma once
#include <list>

#include "brick_game/globals.h"
#include "brick_game/models/frontendInterface.h"

namespace s21 {
class SnakeFrontendData : public Frontend_Interface {
 public:
  virtual std::list<Position> GetBody() = 0;
  virtual int GetDirection() = 0;
};
}  // namespace s21
