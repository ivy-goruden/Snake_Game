#include "snake.hpp"

#include "brick_game/globals.h"

namespace s21 {

template <size_t width, size_t height>
Snake_Game<width, height>::Snake_Game() {
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

template <size_t width, size_t height>
GameModel* s21::Snake_Game<width, height>::updateCurrentState(
    UserAction_t action) {
  const auto& triggers = this->FSM_Triggers[this->cur_state];
  if (this->cur_state < SIZE) {
    s21::Snake_Game<width, height>::Handler main_func =
        this->FSM_Handlers[cur_state];
    if (main_func) (this->*main_func)(action);  // выполнение функции состояния
    for (std::size_t i = 0; i < triggers.size(); ++i) {
      if (triggers[i] && (this->*triggers[i])()) {
        this->cur_state = s21::Snake_Game<width, height>::State(i);
        return this;
      }
    }
  }
  return this;
}

template <size_t width, size_t height>
void s21::Snake_Game<width, height>::Move_Handler(UserAction_t action) {
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

template <size_t width, size_t height>
void Snake_Game<width, height>::Over_Handler(UserAction_t) {}

template <size_t width, size_t height>
void s21::Snake_Game<width, height>::Eating_Handler(UserAction_t) {
  this->ateApple = true;
  this->NewApple();
}

template <size_t width, size_t height>
bool s21::Snake_Game<width, height>::FoundApple() {
  return this->body.front() == this->apple;
}

template <size_t width, size_t height>
bool s21::Snake_Game<width, height>::Smashed() {
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

template <size_t width, size_t height>
bool s21::Snake_Game<width, height>::isWin() {
  return body.size() >=
         static_cast<std::size_t>(s21::Snake_Game<width, height>::HEIGHT) *
             static_cast<std::size_t>(s21::Snake_Game<width, height>::WIDTH);
}

template <size_t width, size_t height>
void s21::Snake_Game<width, height>::Forward() {
  Position headPos = getNextPos();

  this->body.push_front(headPos);
  if (!this->ateApple) {
    this->body.pop_back();
  }
  this->ateApple = false;
}

template <size_t width, size_t height>
Position s21::Snake_Game<width, height>::getNextPos() {
  Position headPos;
  Position prevPos = this->body.front();
  switch (this->direction) {
    case s21::Snake_Game<width, height>::LEFT:
      headPos = Position{prevPos.x - 1, prevPos.y};
      break;
    case s21::Snake_Game<width, height>::RIGHT:
      headPos = Position{prevPos.x + 1, prevPos.y};
      break;
    case s21::Snake_Game<width, height>::UP:
      headPos = Position{prevPos.x, prevPos.y - 1};
      break;
    case s21::Snake_Game<width, height>::DOWN:
      headPos = Position{prevPos.x, prevPos.y + 1};
      break;
  }
  return headPos;
}

template <size_t width, size_t height>
bool s21::Snake_Game<width, height>::isOver() {
  return isWin() || Smashed();
}

template <size_t width, size_t height>
void s21::Snake_Game<width, height>::NewApple() {
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

template <size_t width, size_t height>
bool Snake_Game<width, height>::Yes() {
  return true;
}

template <size_t width, size_t height>
bool Snake_Game<width, height>::No() {
  return false;
}

template <size_t width, size_t height>
std::list<Position> Snake_Game<width, height>::GetBody() const {
  return this->body;
}

// Explicit instantiation
template class s21::Snake_Game<10, 20>;

}  // namespace s21
