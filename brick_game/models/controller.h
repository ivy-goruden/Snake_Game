#pragma once
#include <sys/time.h>

#include <cstddef>

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
  void TerminateHandler();
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

// чтобы убить ncurses если это не удастся
// родителям(Snake_Render, Tetris_Render)
void s21::Controller::TerminateHandler() {
  printf("Работает перехват ошибок\n");
  try {
    if (std::current_exception()) {
      std::rethrow_exception(std::current_exception());
    }
  } catch (const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception caught in terminate handler." << std::endl;
  }
  if (this->render != nullptr) {
    delete this->render;
  }
  if (this->model != nullptr) {
    delete this->model;
  }
  std::abort();
}
}  // namespace s21
