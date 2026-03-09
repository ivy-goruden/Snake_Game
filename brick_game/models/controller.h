#include "gameModel.h"
#include "render.h"
namespace S21 {

class Controller {
 protected:
  GameModel* model;
  Render* render;
  bool running = true;
  long lastTime = {0};

 public:
  // Pure virtual function
  void Run();
  void Quit();
  bool Tick();
  Controller(GameModel* model, Render* render){
    this->model = model;
    this->render = render;
  }
  ~Controller() = default;
};

void Controller::Quit(){
  running = false;
}

void Controller::Run(){
  UserAction_t action = render->GetAction();
  if action{
    GameModel *newGame = model->updateCurrentState(action);
    c.render->UpdateState(newGame);
  }
  if this->Tick(){
    GameModel *newGame = model->updateCurrentState(Tick);
    c.render->UpdateState(newGame);
  }
}

void Controller::Tick(){
  long interval = this->model->GetTimer()
  //нет таймера
  if interval == 0{
    return 0;
  }
  struct timeval now;
  gettimeofday(&now, NULL);

  // Таймер падения фигуры
  if (now.tv_sec * 1000000 + now.tv_usec -
          (lastTime.tv_sec * 1000000 + lastTime.tv_usec) >
      interval) {
    this->lastTime = now;
    return 1;
  }
  return 0;
}
}  // namespace S21
