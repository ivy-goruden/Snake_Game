#include "snake.hpp"

#include "brick_game/globals.h"

namespace s21 {

Snake_Game::Snake_Game() {
  this->lenght = START_SEG;
  this->direction = RIGHT;
  this->cur_state = ST_MOVE;
  this->ateApple = false;
  this->timer = 1000000;
  this->width = WIDTH;
  this->height = HEIGHT;
  InitBody();
  Get_HIScore();
  InitFSM();
  NewApple();
}
// инициализация тела перед началом игры
void s21::Snake_Game::InitBody() {
  int center_y = Height() / 2 - 1;
  int center_x = Width() / 2 - 1;
  for (int i = 0; i < START_SEG; i++) {
    Position newSeg = Position{center_x, center_y + i};
    this->body.push_back(newSeg);
  }
}

Frontend_Interface* s21::Snake_Game::updateCurrentState(UserAction_t action) {
  const auto& triggers = this->FSM_Triggers[this->cur_state];
  if (this->cur_state < SIZE) {
    s21::Snake_Game::Handler main_func = this->FSM_Handlers[cur_state];
    if (main_func) (this->*main_func)(action);  // выполнение функции состояния
    for (std::size_t i = 0; i < triggers.size(); ++i) {
      if (triggers[i] && (this->*triggers[i])()) {
        this->cur_state = s21::Snake_Game::State(i);
        return this;
      }
    }
  }
  return this;
}

bool s21::Snake_Game::isWin() {
  return body.size() >= static_cast<std::size_t>(this->height) *
                            static_cast<std::size_t>(this->width);
}

void s21::Snake_Game::Forward() {
  Position headPos = getNextPos();

  this->body.push_front(headPos);
  if (!this->ateApple) {
    this->body.pop_back();
  }
  this->ateApple = false;
}

Position s21::Snake_Game::getNextPos() {
  Position headPos;
  Position prevPos = this->body.front();
  Direction dir = this->direction;
  switch (dir) {
    case LEFT:
      headPos = Position{prevPos.x - 1, prevPos.y};
      break;
    case RIGHT:
      headPos = Position{prevPos.x + 1, prevPos.y};
      break;
    case UP:
      headPos = Position{prevPos.x, prevPos.y - 1};
      break;
    case DOWN:
      headPos = Position{prevPos.x, prevPos.y + 1};
      break;
  }
  return headPos;
}

void s21::Snake_Game::NewApple() {
  std::list<Position> availableSpots;
  for (int i = 0; i < this->width; ++i) {
    for (int x = 0; x < this->height; ++x) {
      Position cur = Position{i, x};
      bool available = true;
      for (const auto& pos : this->body) {
        if (pos == cur) {
          available = false;
          break;
        }
      }
      if (available) {
        availableSpots.push_back(cur);
      }
    }
  }
  if (!availableSpots.empty()) {
    int randomN = random(0, int(availableSpots.size()) - 1);
    auto it = availableSpots.begin();
    std::advance(it, randomN);
    this->apple = *it;
  }
}

bool Snake_Game::Yes() { return true; }

bool Snake_Game::No() { return false; }

void Snake_Game::Save_HIScore() {
  FILE* file = fopen("brick_game/snake/score.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  int score = std::max(this->score, this->highScore);
  fprintf(file, "%d", score);
  fclose(file);
}

void Snake_Game::Get_HIScore() {
  FILE* file = fopen("brick_game/snake/score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &this->highScore) != 1) {
      this->highScore = 0;
    }
    fclose(file);
  } else {
    this->highScore = 0;
  }
}

void Snake_Game::InitFSM() {
  // Initialize FSM Triggers
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      this->FSM_Triggers[i][j] = &Snake_Game::No;
    }
  }
  this->FSM_Triggers[ST_MOVE][ST_OVER] = &Snake_Game::isOver;
  this->FSM_Triggers[ST_MOVE][ST_EATING] = &Snake_Game::FoundApple;
  this->FSM_Triggers[ST_EATING][ST_MOVE] = &Snake_Game::Yes;

  // Initialize FSM Handlers
  this->FSM_Handlers[ST_MOVE] = &Snake_Game::Move_Handler;
  this->FSM_Handlers[ST_OVER] = &Snake_Game::Over_Handler;
  this->FSM_Handlers[ST_EATING] = &Snake_Game::Eating_Handler;
}

std::list<Position> Snake_Game::GetBody() { return this->body; }
Direction Snake_Game::GetDirection() { return this->direction; }
Position Snake_Game::GetApple() const { return this->apple; }
bool Snake_Game::IsWin() { return this->isWin(); }
bool Snake_Game::IsLose() { return this->isOver(); }
void Snake_Game::reset() {
  this->lenght = START_SEG;
  this->direction = RIGHT;
  this->cur_state = ST_MOVE;
  this->ateApple = false;
  this->timer = 1000000;
  this->score = 0;
  this->level = 0;
  this->body.clear();
  Get_HIScore();
  InitBody();
  NewApple();
}
}  // namespace s21
