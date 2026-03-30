#include "tetris_desktop_render.h"

bool s21::Tetris_Desktop_Render::Waiting_Handler(int key) {
  switch (key) {
    case Qt::Key_S:
      this->cur_state = ST_MOVE;
  }
  return true;
};
bool s21::Tetris_Desktop_Render::Move_Handler(int key) {
  switch (key) {
    case Qt::Key_P:
      this->cur_state = ST_PAUSE;
      return true;
  }
  return false;
};
bool s21::Tetris_Desktop_Render::Pause_Handler(int key) {
  switch (key) {
    case Qt::Key_P:
      this->cur_state = ST_MOVE;
      return true;
  }
  return false;
};
bool s21::Tetris_Desktop_Render::Win_Handler(int key) {
  switch (key) {
    case Qt::Key_Enter:
      this->cur_state = ST_WAITING;
  }
  return true;
};
bool s21::Tetris_Desktop_Render::Lose_Handler(int key) {
  switch (key) {
    case Qt::Key_Enter:
      this->cur_state = ST_WAITING;
  }
  return true;
};