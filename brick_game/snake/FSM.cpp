#include <algorithm>

#include "brick_game/globals.h"
#include "snake.hpp"

namespace s21 {
void s21::Snake_Game::Move_Handler(UserAction_t action) {
  write_log("CHANGING DIRECTION");
  switch (action) {
    case Left:
      if (this->direction != static_cast<Direction>(RIGHT))
        this->direction = static_cast<Direction>(LEFT);
      break;
    case Right:
      if (this->direction != static_cast<Direction>(LEFT))
        this->direction = static_cast<Direction>(RIGHT);
      break;
    case Up:
      if (this->direction != static_cast<Direction>(DOWN))
        this->direction = static_cast<Direction>(UP);
      break;
    case Down:
      if (this->direction != static_cast<Direction>(UP))
        this->direction = static_cast<Direction>(DOWN);
      break;
    case Action:
      write_log("ACTION is pressed");
      this->Forward();
      break;
    case Tick:
      write_log("TICK has happened");
      this->Forward();
      break;
    default:
      break;
  }
}

void Snake_Game::Over_Handler(UserAction_t action) {
  switch (action) {
    case Enter:
      this->reset();
      break;
    default:
      break;
  }
}

void s21::Snake_Game::Eating_Handler(UserAction_t) {
  this->score += 1;
  this->level = std::min(int(this->score / 5.0), 10);
  this->timer *= 1 - 0.1 * this->level;
  this->ateApple = true;
  this->NewApple();
}

bool s21::Snake_Game::FoundApple() { return this->body.front() == this->apple; }

bool s21::Snake_Game::Smashed() {
  Position headPos = this->body.front();
  bool SmashedIntoSelf = false;
  bool SmashedIntoBorder = (headPos.x < 0 || headPos.x >= this->width) ||
                           (headPos.y < 0 || headPos.y >= this->height);
  for (auto it = std::next(body.begin()); it != body.end(); ++it) {
    if (*it == headPos) {
      SmashedIntoSelf = true;
      break;
    }
  }
  return SmashedIntoBorder || SmashedIntoSelf;
}

bool s21::Snake_Game::isOver() {
  if (isWin() || Smashed()) {
    this->Save_HIScore();
    return true;
  }
  return false;
}
}  // namespace s21
