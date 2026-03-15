#pragma once

#include "../models/frontendInterface.h"

namespace s21 {

typedef std::vector<std::vector<int>> matrix_t;

struct figure_t {
  int x;
  int y;
  matrix_t figure;
  int rotation;
};

class TetrisFrontendData : public Frontend_Interface {
 protected:
  figure_t nextFigure;

 public:
  virtual matrix_t GetNextFigure() { return nextFigure.figure; };
  virtual matrix_t GetField() = 0;
};
}  // namespace s21
