#pragma once
#include <iostream>

#include "brick_game/globals.h"
#include "brick_game/models/gameModel.h"
namespace s21 {

class Render {
 protected:
  s21::GameModel* model;

 public:
  virtual UserAction_t GetAction() = 0;
  virtual void UpdateState(s21::GameModel* model) = 0;

  virtual ~Render() = default;
};

}  // namespace s21
