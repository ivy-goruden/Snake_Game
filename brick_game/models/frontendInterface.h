#pragma once
#include <list>

#include "../globals.h"
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
  int GetScore() const { return this->score; }
  int GetHighScore() const { return this->highScore; }
  int GetLevel() const { return this->level; }
  virtual void Save_HIScore() = 0;
  virtual void Get_HIScore() = 0;
  virtual ~Frontend_Interface() = default;
};
}  // namespace s21
