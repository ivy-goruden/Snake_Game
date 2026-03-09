#include "gameModel.h"
#include "render.h"
namespace s21 {

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

void s21::Controller::Quit(){
  running = false;
}

void s21::Controller::Run(){
  while (true){
    UserAction_t action = this->render->GetAction();
    if (action){
      if (action == Terminate){
        return;
      }
      GameModel *newGame = this->model->updateCurrentState(action);
      c.render->UpdateState(newGame);
    }
    if (this->Tick()){
      GameModel *newGame = this->model->updateCurrentState(Tick);
      c.render->UpdateState(newGame);
    }
  }
}

bool s21::Controller::Tick(){
  long interval = this->model->GetTimer()
  //нет таймера
  if interval == 0{
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
}  // namespace S21
