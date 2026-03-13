#include "brick_game/globals.h"
#include "gui/desktop/snake.hpp"

namespace s21 {
void Snake_Render::WaitingScreen_Handler() {
  const char* msg = "PRESS S";
  const char* msg2 = "TO START ";
  int x = (this->width - sizeof(msg) + 1) / 2;
  int y = this->height / 2;
  mvwprintw(this->window, y, x, msg);
  x = (this->width - sizeof(msg2) + 1) / 2;
  mvwprintw(this->window, y + 1, x, msg2);
}

void Snake_Render::MoveScreen_Handler() {
  int rowsPerSeg = this->height / this->model->Height();
  int colsPerSeg = this->width / this->model->Width();
  Position apple = this->model->GetApple();
  write_log("Apple at: %d %d", apple.x, apple.y);
  for (int i = 0; i < rowsPerSeg; ++i) {
    for (int j = 0; j < colsPerSeg; ++j) {
      mvwprintw(this->window, apple.y * rowsPerSeg + i + 1,
                apple.x * colsPerSeg + j + 1, "@");
    }
  }
  auto body = this->model->GetBody();
  for (auto it = body.begin(); it != body.end(); ++it) {
    Position pos = *it;
    const char* el = "";
    if (it == body.begin()) {
      switch (this->model->GetDirection()) {
        case UP:
          el = "^";
          break;
        case DOWN:
          el = "v";
          break;
        case LEFT:
          el = "<";
          break;
        case RIGHT:
          el = ">";
          break;
      }
    } else {
      el = "O";
    }
    for (int i = 0; i < rowsPerSeg; ++i) {
      for (int j = 0; j < colsPerSeg; ++j) {
        mvwprintw(this->window, pos.y * rowsPerSeg + i + 1,
                  pos.x * colsPerSeg + j + 1, el);
      }
    }
  }
}

void Snake_Render::PauseScreen_Handler() {
  const char* msg = "PAUSED";
  int x = (this->width - sizeof(msg) + 1) / 2;
  int y = this->height / 2;
  mvwprintw(this->window, y, x, msg);
}

void Snake_Render::WinScreen_Handler() {
  const char* msg = "WIN";
  const char* msg1 = "PRESS ENTER";
  const char* msg2 = "TO RESTART";
  int x = (this->width - sizeof(msg) + 1) / 2;
  int y = this->height / 2 - 1;
  mvwprintw(this->window, y, x, msg);

  x = (this->width - sizeof(msg1) + 1) / 2;
  mvwprintw(this->window, y + 1, x, msg1);

  x = (this->width - sizeof(msg2) + 1) / 2;
  mvwprintw(this->window, y + 2, x, msg2);
}

void Snake_Render::LoseScreen_Handler() {
  const char* msg = "LOSE";
  const char* msg1 = "PRESS ENTER";
  const char* msg2 = "TO RESTART";
  int x = (this->width - sizeof(msg) + 1) / 2;
  int y = this->height / 2 - 1;
  mvwprintw(this->window, y, x, msg);

  x = (this->width - sizeof(msg1) + 1) / 2;
  mvwprintw(this->window, y + 1, x, msg1);

  x = (this->width - sizeof(msg2) + 1) / 2;
  mvwprintw(this->window, y + 2, x, msg2);
}
}  // namespace s21