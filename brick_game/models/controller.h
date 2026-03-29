#pragma once
#include <sys/time.h>

#include <cstddef>
#include <memory>

#include "../globals.h"
#include "gameModel.h"
#include "render.h"
namespace s21 {

class Controller {
 protected:
  std::unique_ptr<s21::GameModel> model;
  std::unique_ptr<Render> render;

 protected:
  bool running = true;
  timeval lastTime = {0, 0};

 public:
  // Pure virtual function
  virtual void Run() = 0;
  void Quit();
  virtual bool Tick() = 0;
  void TerminateHandler();
  Controller() {}
  virtual ~Controller() = 0;
};
}  // namespace s21
