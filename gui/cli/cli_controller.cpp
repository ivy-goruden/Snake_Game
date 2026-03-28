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