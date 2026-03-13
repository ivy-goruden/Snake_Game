#include "tetris.hpp"
namespace s21 {

void Tetris_Game::UpdateScore(int rows) {
  if (rows == 1) {
    this->score += 100;
  } else if (rows == 2) {
    this->score += 300;
  } else if (rows == 3) {
    this->score += 700;
  } else if (rows == 4) {
    this->score += 1500;
  }
  this->level = (int)(this->score / 600);
  this->timer = 1 - this->level * 0.1;
}

void Tetris_Game::InitFSM() {
  // Initialize FSM Triggers
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      this->FSM_Triggers[i][j] = &Tetris_Game::No;
    }
  }
  this->FSM_Triggers[ST_SPAWN][ST_FALLING] = &Tetris_Game::Yes;
  // this->FSM_Triggers[ST_SPAWN][ST_FALLING] = &Tetris_Game::NoFigure;
  this->FSM_Triggers[ST_FALLING][ST_LOCK] = &Tetris_Game::isCollision;
  this->FSM_Triggers[ST_LOCK][ST_SPAWN] = &Tetris_Game::Yes;
  this->FSM_Triggers[ST_LOCK][ST_OVER] = &Tetris_Game::isOver;

  // Initialize FSM Handlers
  this->FSM_Handlers[ST_SPAWN] = &Tetris_Game::Spawn_Handler;
  this->FSM_Handlers[ST_FALLING] = &Tetris_Game::Falling_Handler;
  this->FSM_Handlers[ST_LOCK] = &Tetris_Game::Lock_Handler;
  this->FSM_Handlers[ST_OVER] = &Tetris_Game::Over_Handler;
}

void Tetris_Game::Spawn_Handler(UserAction_t action) {};

Frontend_Interface* s21::Tetris_Game::updateCurrentState(UserAction_t action) {
  const auto& triggers = this->FSM_Triggers[this->cur_state];
  if (this->cur_state < SIZE) {
    s21::Tetris_Game::Handler main_func = this->FSM_Handlers[cur_state];
    if (main_func) (this->*main_func)(action);  // выполнение функции состояния
    for (std::size_t i = 0; i < triggers.size(); ++i) {
      if (triggers[i] && (this->*triggers[i])()) {
        this->cur_state = s21::Tetris_Game::State(i);
        return this;
      }
    }
  }
  return this;
}

bool Tetris_Game::isCollision() { return 0; }
bool Tetris_Game::isOver() { return this->isWin() || this->isOverflowed(); }

}  // namespace s21