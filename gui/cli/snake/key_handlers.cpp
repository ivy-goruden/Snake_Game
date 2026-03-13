#include <type_traits>

#include "brick_game/globals.h"
#include "gui/desktop/snake.hpp"
namespace s21 {

bool Snake_Render::Waiting_Handler(int key) {
  if (key == 's') {
    ChangeState(ST_MOVE);
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}

bool Snake_Render::Move_Handler(int key) {
  if (key == 'p') {
    ChangeState(ST_PAUSE);
    return true;
  }
  if (this->model->IsWin()) {
    ChangeState(ST_WIN);
  }
  if (this->model->IsLose()) {
    ChangeState(ST_LOSE);
  }
  return false;
}

bool Snake_Render::Pause_Handler(int key) {
  if (key == 'p') {
    ChangeState(ST_MOVE);
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}

bool Snake_Render::Win_Handler(int key) {
  if (key == KEY_ENTER || key == '\n') {
    ChangeState(ST_WAITING);
    return false;
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}

bool Snake_Render::Lose_Handler(int key) {
  if (key == KEY_ENTER || key == '\n') {
    ChangeState(ST_WAITING);
    return false;
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}
}  // namespace s21