#pragma once

#include "../../brick_game/globals.h"
#include "../../brick_game/snake/snake.hpp"

namespace s21 {

class Snake_Test : public s21::Snake_Game {
 public:
  using s21::Snake_Game::Forward;
  using s21::Snake_Game::GetApple;
  using s21::Snake_Game::GetBody;
  using s21::Snake_Game::getNextPos;
  using s21::Snake_Game::NewApple;
  using s21::Snake_Game::SetDirection;
  using s21::Snake_Game::Smashed;
};
}  // namespace s21
