#include "snake.hpp"

#include "brick_game/globals.h"

namespace s21 {

Snake_Game::Snake_Game() {
  this->lenght = 4;
  this->direction = RIGHT;
  this->cur_state = ST_MOVE;
  this->ateApple = false;
  this->timer = 1000000;

  // Initialize body
  for (int i = 0; i < this->lenght; ++i) {
    this->body.push_back({HEIGHT / 2, WIDTH / 2 - i});
  }

  // Initialize FSM Triggers
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      this->FSM_Triggers[i][j] = &Snake_Game::No;
    }
  }

  this->FSM_Triggers[ST_MOVE][ST_OVER] = &Snake_Game::isOver;
  this->FSM_Triggers[ST_MOVE][ST_EATING] = &Snake_Game::FoundApple;

  this->FSM_Triggers[ST_OVER][ST_OVER] = &Snake_Game::Yes;

  this->FSM_Triggers[ST_EATING][ST_MOVE] = &Snake_Game::Yes;

  // Initialize FSM Handlers
  this->FSM_Handlers[ST_MOVE] = &Snake_Game::Move_Handler;
  this->FSM_Handlers[ST_OVER] = &Snake_Game::Over_Handler;
  this->FSM_Handlers[ST_EATING] = &Snake_Game::Eating_Handler;

  this->NewApple();
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

void s21::Snake_Game::Move_Handler(UserAction_t action) {
  switch (action) {
    case Left:
      this->direction = (this->direction + 3) % 4;
      break;
    case Right:
      this->direction = (this->direction + 1) % 4;
      break;
    case Action:
      this->Forward();
      break;
    case Tick:
      this->Forward();
      break;
    default:
      break;
  }
}

void Snake_Game::Over_Handler(UserAction_t) {}

void s21::Snake_Game::Eating_Handler(UserAction_t) {
  this->ateApple = true;
  this->NewApple();
}

bool s21::Snake_Game::FoundApple() { return this->body.front() == this->apple; }

bool s21::Snake_Game::Smashed() {
  Position headPos = this->body.front();
  bool SmashedIntoSelf = false;
  bool SmashedIntoBorder = (headPos.x < 0 || headPos.x >= WIDTH) ||
                           (headPos.y < 0 || headPos.y >= HEIGHT);
  for (auto it = std::next(body.begin()); it != body.end(); ++it) {
    if (*it == headPos) {
      SmashedIntoSelf = true;
      break;
    }
  }
  return SmashedIntoBorder || SmashedIntoSelf;
}

bool s21::Snake_Game::isWin() {
  return body.size() >= static_cast<std::size_t>(this->HEIGHT) *
                            static_cast<std::size_t>(this->WIDTH);
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
  switch (this->direction) {
    case s21::Snake_Game::LEFT:
      headPos = Position{prevPos.x - 1, prevPos.y};
      break;
    case s21::Snake_Game::RIGHT:
      headPos = Position{prevPos.x + 1, prevPos.y};
      break;
    case s21::Snake_Game::UP:
      headPos = Position{prevPos.x, prevPos.y - 1};
      break;
    case s21::Snake_Game::DOWN:
      headPos = Position{prevPos.x, prevPos.y + 1};
      break;
  }
  return headPos;
}

bool s21::Snake_Game::isOver() { return isWin() || Smashed(); }

void s21::Snake_Game::NewApple() {
  std::list<Position> availableSpots;
  for (int i = 0; i < this->HEIGHT; ++i) {
    for (int x = 0; x < this->WIDTH; ++x) {
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

std::list<Position> Snake_Game::GetBody() { return this->body; }

int Snake_Game::GetDirection() { return this->direction; }
int Snake_Game::Width() const { return this->width; }
int Snake_Game::Height() const { return this->height; }
int Snake_Game::GetScore() const { return this->score; }
int Snake_Game::GetHighScore() const { return this->highScore; }
int Snake_Game::GetLevel() const { return this->level; }
}  // namespace s21
