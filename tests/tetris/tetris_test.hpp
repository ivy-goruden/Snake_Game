#pragma once

#include "../../brick_game/globals.h"
#include "../../brick_game/tetris/tetris.hpp"

namespace s21 {

class Tetris_Test : public s21::Tetris_Game {
 public:
  using s21::Tetris_Game::AccelerateFigure;
  using s21::Tetris_Game::collision;
  using s21::Tetris_Game::Falling_Handler;
  using s21::Tetris_Game::GetField;
  using s21::Tetris_Game::isCollision;
  using s21::Tetris_Game::isOver;
  using s21::Tetris_Game::isOverflowed;
  using s21::Tetris_Game::isWin;
  using s21::Tetris_Game::Lock_Handler;
  using s21::Tetris_Game::New_Figure;
  using s21::Tetris_Game::Over_Handler;
  using s21::Tetris_Game::RotateFigure;
  using s21::Tetris_Game::Spawn_Handler;
  using s21::Tetris_Game::UpdateScore;
  using s21::Tetris_Game::ValidateFixed;
  using State = s21::Tetris_Game::State;

  State GetState() { return this->cur_state; }
  void SetState(State state) { this->cur_state = state; }

  void SetFigure(figure_t figure) { this->figure = figure; }
  figure_t GetFigure() { return this->figure; }
  void SetFixed(std::array<std::array<int, WIDTH>, HEIGHT> fixed) {
    this->fixed = fixed;
  }
  std::array<std::array<int, WIDTH>, HEIGHT> GetFixed() { return this->fixed; }
  void SetScore(int score) { this->score = score; }
  void SetHIScore(int score) { this->highScore = score; }
  int GetScore() { return this->score; }
  int GetLevel() { return this->level; }
  void SetNextFigure(figure_t figure) { this->nextFigure = figure; }
  figure_t GetNextFigureObj() { return this->nextFigure; }
};
}  // namespace s21