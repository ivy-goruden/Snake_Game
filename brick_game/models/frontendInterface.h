#pragma once
#include <list>

#include "brick_game/globals.h"
namespace s21 {
class Frontend_Interface {
 public:
  virtual int Width() const = 0;
  virtual int Height() const = 0;
  virtual int GetScore() const = 0;
  virtual int GetHighScore() const = 0;
  virtual int GetLevel() const = 0;
  virtual ~Frontend_Interface() = default;
};
}  // namespace s21
