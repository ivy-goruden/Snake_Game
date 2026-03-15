#include "tetris.hpp"

#include <array>
#include <cstddef>
#include <cstdlib>

#include "frontend.hpp"
namespace s21 {

Tetris_Game::Tetris_Game() {
  this->timer = 1000000;
  InitFSM();
  prepare_figures();
  for (auto& row : fixed) {
    row.fill(0);
  }
  this->cur_state = ST_SPAWN;
  New_Figure(this->nextFigure);
  this->Get_HIScore();
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

Frontend_Interface* s21::Tetris_Game::updateCurrentState(UserAction_t action) {
  write_log("key is %d", action);
  write_log("Game state is: %d", this->cur_state);
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

bool Tetris_Game::isCollision() {
  return this->collision(this->figure.x, this->figure.y + 1,
                         this->figure.figure);
}

bool Tetris_Game::collision(int newX, int newY, matrix_t& shape) {
  for (size_t i = 0; i < shape.size(); i++) {
    for (size_t j = 0; j < shape[i].size(); j++) {
      if (shape[i][j]) {
        int x = newX + j;
        int y = newY + i;
        if (x < 0 || x >= WIDTH || y >= HEIGHT) return true;
        if (y >= 0 && this->fixed[y][x]) return true;
      }
    }
  }
  return false;
}
bool Tetris_Game::isOver() {
  if (this->isWin() || this->isOverflowed()) {
    this->Save_HIScore();
    return true;
  }
  return false;
}

bool Tetris_Game::IsWin() {
  if (this->isWin()) {
    return true;
  }
  return false;
}

bool Tetris_Game::IsLose() {
  if (!this->isWin() && this->isOver()) {
    return true;
  }
  return false;
}

matrix_t Tetris_Game::GetField() {
  matrix_t result = matrix_t(HEIGHT, std::vector<int>(WIDTH));
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      result[i][j] = this->fixed[i][j];
    }
  }
  for (size_t i = 0; i < this->figure.figure.size(); i++) {
    for (size_t j = 0; j < this->figure.figure[i].size(); j++) {
      if (this->figure.figure[i][j]) {
        int y = this->figure.y + i;
        int x = this->figure.x + j;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
          result[y][x] = 1;
        }
      }
    }
  }
  return result;
}

void Tetris_Game::reset() {
  this->cur_state = ST_SPAWN;
  this->score = 0;
  this->level = 0;
  for (auto& row : fixed) {
    row.fill(0);
  }
  this->Get_HIScore();
}

bool Tetris_Game::isWin() { return this->score > 5000; }

bool Tetris_Game::isOverflowed() {
  for (int x = 0; x < WIDTH; ++x) {
    if (fixed[0][x] != 0) return true;
  }
  return false;
}
void Tetris_Game::Save_HIScore() {
  FILE* file = fopen("brick_game/tetris/score.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  int score = std::max(this->score, this->highScore);
  fprintf(file, "%d", score);
  fclose(file);
}

void Tetris_Game::Get_HIScore() {
  FILE* file = fopen("brick_game/tetris/score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &this->highScore) != 1) {
      this->highScore = 0;
    }
    fclose(file);
  } else {
    this->highScore = 0;
  }
}

}  // namespace s21