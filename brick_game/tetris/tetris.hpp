#pragma once

#include <array>

#include "../globals.h"
#include "../models/gameModel.h"
#include "frontend.hpp"
namespace s21 {

class Tetris_Game : public s21::GameModel, public s21::TetrisFrontendData {
 protected:
  std::array<std::array<int, WIDTH>, HEIGHT> fixed;
  figure_t figure;

  typedef enum {
    ST_OVER,
    ST_SPAWN,
    ST_FALLING,
    ST_LOCK,
    SIZE,
  } State;

  typedef bool (Tetris_Game::*Trigger)();
  typedef void (Tetris_Game::*Handler)(UserAction_t);

  std::array<std::array<Trigger, SIZE>, SIZE> FSM_Triggers;
  std::array<Handler, SIZE> FSM_Handlers;
  State cur_state;

  // helpers
  void prepare_figures();
  void InitFSM() override;
  void UpdateScore(int);
  void New_Figure(figure_t&);
  void AccelerateFigure();
  void RotateFigure();
  void ValidateFixed();
  void StackFixed();
  // triggers

  bool isCollision();
  bool collision(int x, int y, matrix_t& shape);
  bool isWin();
  bool isOverflowed();
  bool isOver();

  // handlers
  void Spawn_Handler(UserAction_t);
  void Falling_Handler(UserAction_t);
  void Lock_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void reset() override;

  // frontend
  matrix_t GetField() override;
  bool IsWin() override;
  bool IsLose() override;
  void Save_HIScore() override;
  void Get_HIScore() override;

 public:
  Frontend_Interface* updateCurrentState(UserAction_t action) override;
  Tetris_Game();
  std::list<figure_t> figures;
};
}  // namespace s21