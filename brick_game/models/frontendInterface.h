#pragma once
#include <list>

#include "brick_game/globals.h"
namespace s21 {
class Frontend_Interface {
 protected:
  int score = 0;
  int highScore = 0;
  int level = 0;
  int width = 0;
  int height = 0;

 public:
  virtual bool IsWin() = 0;
  virtual bool IsLose() = 0;
  int Width() const { return this->width; }
  int Height() const { return this->height; }
  int GetScore() const { return this->score; }
  int GetHighScore() const { return this->highScore; }
  int GetLevel() const { return this->level; }
  virtual ~Frontend_Interface() = default;
};
}  // namespace s21
