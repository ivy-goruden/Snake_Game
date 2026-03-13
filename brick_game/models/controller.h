#pragma once
#include <sys/time.h>

#include "../globals.h"
#include "gameModel.h"
#include "render.h"
namespace s21 {

class Controller {
 protected:
  s21::GameModel* model;
  Render* render;
  bool running = true;
  timeval lastTime = {0, 0};

 public:
  // Pure virtual function
  void Run();
  void Quit();
  bool Tick();
  Controller(GameModel* model, Render* render) {
    this->model = model;
    this->render = render;
  }
  ~Controller() = default;
};

void s21::Controller::Quit() { running = false; }

void s21::Controller::Run() {
  while (true) {
    UserAction_t action = this->render->GetAction();
    if (action) {
      if (action == Terminate) {
        return;
      }
      Frontend_Interface* newGame = this->model->updateCurrentState(action);
      this->render->UpdateState(newGame);
    }
    if (this->Tick()) {
      Frontend_Interface* newGame = this->model->updateCurrentState(::Tick);
      this->render->UpdateState(newGame);
    }
  }
}

bool s21::Controller::Tick() {
  long interval = this->model->GetTimer();
  // нет таймера
  if (interval == 0) {
    return 0;
  }
  struct timeval now;
  gettimeofday(&now, NULL);

  // Таймер падения фигуры
  if (now.tv_sec * 1000000 + now.tv_usec -
          (this->lastTime.tv_sec * 1000000 + lastTime.tv_usec) >
      interval) {
    this->lastTime = now;
    return 1;
  }
  return 0;
}
}  // namespace s21
