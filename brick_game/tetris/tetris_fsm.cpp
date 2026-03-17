#include <vector>

#include "../globals.h"
#include "frontend.hpp"
#include "tetris.hpp"

namespace s21 {

void Tetris_Game::Spawn_Handler(UserAction_t) {
  this->figure = this->nextFigure;
  this->New_Figure(this->nextFigure);
}
void Tetris_Game::Falling_Handler(UserAction_t action) {
  switch (action) {
    case Left:
      if (!collision(this->figure.x - 1, this->figure.y, this->figure.figure)) {
        this->figure.x -= 1;
      }
      break;
    case Right:
      if (!collision(this->figure.x + 1, this->figure.y, this->figure.figure)) {
        this->figure.x += 1;
      }
      break;
    case Down:
      this->AccelerateFigure();
      break;
    case Action:
      this->RotateFigure();
      break;
    case Tick:
      if (!collision(this->figure.x, this->figure.y + 1, this->figure.figure)) {
        this->figure.y += 1;
      }
      break;
    default:
      break;
  }
}

void Tetris_Game::Lock_Handler(UserAction_t) {
  for (size_t i = 0; i < this->figure.figure.size(); i++) {
    for (size_t j = 0; j < this->figure.figure[i].size(); j++) {
      if (this->figure.figure[i][j]) {
        int y = this->figure.y + i;
        int x = this->figure.x + j;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
          this->fixed[y][x] = 1;
        }
      }
    }
  }
  this->ValidateFixed();
}

void Tetris_Game::Over_Handler(UserAction_t action) {
  if (action == Enter) {
    this->reset();
  }
}

}  // namespace s21
