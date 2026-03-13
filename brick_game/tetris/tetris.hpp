#pragma once

#include <array>

#include "../globals.h"
#include "../models/gameModel.h"
#include "brick_game.h"
#include "frontend.hpp"
namespace s21 {

class Tetris_Game : public s21::GameModel, public s21::TetrisFrontendData {
 protected:
  int width = 10;
  int height = 20;
  long timer = 1000000;
  std::array<int, WIDTH * HEIGHT> fixed;

  typedef enum {
    ST_SPAWN,
    ST_FALLING,
    ST_LOCK,
    ST_OVER,
    SIZE,
  } State;

  typedef bool (Tetris_Game::*Trigger)();
  typedef void (Tetris_Game::*Handler)(UserAction_t);

  std::array<std::array<Trigger, SIZE>, SIZE> FSM_Triggers;
  std::array<Handler, SIZE> FSM_Handlers;
  State cur_state;

  void prepare_figures();
  void InitFSM() override;
  void UpdateScore(int);
  void New_Figure();

  bool NoFigure();
  bool isCollision();
  bool isWin();
  bool isOverflowed();
  bool isOver();
  void Spawn_Handler(UserAction_t);
  void Falling_Handler(UserAction_t);
  void Lock_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void reset() override;

 public:
  Frontend_Interface* updateCurrentState(UserAction_t action) override;
  Tetris_Game() {}
  std::list<figure_t> figures;
};
}  // namespace s21