#pragma once

#include "../../brick_game/globals.h"
#include "../../brick_game/snake/snake.hpp"

namespace s21 {

class Snake_Test : public s21::Snake_Game {
 public:
  using s21::Snake_Game::Forward;
  using s21::Snake_Game::GetApple;
  using s21::Snake_Game::GetBody;
  using s21::Snake_Game::GetLevel;
  using s21::Snake_Game::getNextPos;
  using s21::Snake_Game::GetScore;
  using s21::Snake_Game::NewApple;
  using s21::Snake_Game::Smashed;
  using s21::Snake_Game::ValidateBody;
  using State = s21::Snake_Game::State;

  State GetState() { return this->cur_state; }
  void SetState(State state) { this->cur_state = state; }

  void SetDirection(Direction dir) { this->direction = dir; }
  void SetApple(Position apple) { this->apple = apple; }
  int GetLenght() { return this->lenght; }
  void SetScore(int score) { this->score = score; }
  void SetHIScore(int score) { this->highScore = score; }
  void SetBody(std::list<Position> body) { this->body = body; }
  bool Is_AteApple() { return this->ateApple; }
};
}  // namespace s21
