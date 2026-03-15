#include "tetris.hpp"

namespace s21 {

void Tetris_Render::WaitingScreen_Handler() {
  const char* msg = "PRESS S TO START";
  mvwprintw(this->window, this->height / 2, (this->width - sizeof(msg)) / 2,
            "%s", msg);
}

void Tetris_Render::MoveScreen_Handler() {
  auto field = this->model->GetField();
  for (int i = 0; i < this->height; ++i) {
    for (int j = 0; j < this->width; ++j) {
      if (field[i][j]) {
        mvwaddch(this->window, i + 1, j + 1, '#');
      } else {
        mvwaddch(this->window, i + 1, j + 1, ' ');
      }
    }
  }
}

void Tetris_Render::PauseScreen_Handler() {
  const char* msg = "PAUSED";
  mvwprintw(this->window, this->height / 2, (this->width - sizeof(msg)) / 2,
            "%s", msg);
}

void Tetris_Render::WinScreen_Handler() {
  const char* msg = "YOU WIN";
  mvwprintw(this->window, this->height / 2, (this->width - sizeof(msg)) / 2,
            "%s", msg);
}

void Tetris_Render::LoseScreen_Handler() {
  const char* msg = "GAME OVER";
  mvwprintw(this->window, this->height / 2, (this->width - sizeof(msg)) / 2,
            "%s", msg);
}

}  // namespace s21