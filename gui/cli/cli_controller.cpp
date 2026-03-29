#include "cli_controller.h"
using namespace s21;
void CLI_Controller::Run() {
  while (running) {
    UserAction_t action = this->render->GetAction();
    if (action) {
      if (action == Terminate) {
        this->running = false;
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

bool CLI_Controller::Tick() {
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