#include "tetris.hpp"

namespace s21 {

bool Tetris_Render::Waiting_Handler(int key) {
  if (key == 's') {
    ChangeState(ST_MOVE);
    return true;
  }
  if (key != ERR && key != 'q') {
    return true;
  }
  return false;
}

bool Tetris_Render::Move_Handler(int key) {
  if (key == 'p') {
    ChangeState(ST_PAUSE);
    return true;
  }
  return false;
}

bool Tetris_Render::Pause_Handler(int key) {
  if (key == 'p') {
    ChangeState(ST_MOVE);
    return true;
  }
  if (key != ERR && key != 'q') {
    return true;
  }
  return false;
}

bool Tetris_Render::Win_Handler(int key) {
  if (key == '\n' || key == KEY_ENTER) {
    ChangeState(ST_WAITING);
    return false;
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}

bool Tetris_Render::Lose_Handler(int key) {
  if (key == '\n' || key == KEY_ENTER) {
    ChangeState(ST_WAITING);
    return false;
  }
  if (key != ERR && key != 'q') {
    return true;  // ничего не делаем, если нажата другая клавиша
  }
  return false;
}

}  // namespace s21
