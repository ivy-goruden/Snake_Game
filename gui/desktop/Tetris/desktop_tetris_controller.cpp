#include "desktop_tetris_controller.h"

#include <QTimer>

#include "../../../brick_game/models/controller.h"
#include "tetris_desktop_render.h"
using namespace s21;
QT_Tetris_Controller::QT_Tetris_Controller(Tetris_Game *model,
                                           Tetris_Desktop_Render *render,
                                           QObject *parent)
    : QObject(parent), Controller(), model(model), render(render) {
  connect(render, SIGNAL(keyPressed(UserAction_t)), this,
          SLOT(onKeyPressed(UserAction_t)));
};

void QT_Tetris_Controller::onKeyPressed(UserAction_t key) {
  // Передаем нажатие клавиши в игру
  if (key == Terminate) {
    qApp->exit();
  }
  auto update = this->model->updateCurrentState(key);
  this->render->UpdateState(update);
}

void QT_Tetris_Controller::Run() {
  qDebug() << "Tick";
  qDebug() << this->model->GetTimer();
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &QT_Tetris_Controller::onTick);

  timer->start(this->model->GetTimer() / 1000);
}

void QT_Tetris_Controller::onTick() {
  if (this->render->isRunning()) {
    auto update = this->model->updateCurrentState(::Tick);
    this->render->UpdateState(update);
    qDebug() << "Tick";
  }
}

bool QT_Tetris_Controller::Tick() { return 0; }