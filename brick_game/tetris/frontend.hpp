#pragma once

#include "../models/frontendInterface.h"
#include "brick_game.h"

namespace s21 {

typedef std::vector<std::vector<int>> matrix_t;

typedef struct figure_t {
  int x;
  int y;
  matrix_t figure;
  int rotation;
} figure_t;

class TetrisFrontendData : public Frontend_Interface {
 private:
  figure_t nextFigure;

 public:
  virtual matrix_t GetNextFigure() { return nextFigure.figure; };
  virtual matrix_t GetField() = 0;
};
}  // namespace s21
