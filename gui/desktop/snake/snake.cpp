#include "snake.hpp"

#include "../../../brick_game/globals.h"
using namespace s21;
UserAction_t Snake_Desktop_Render::GetAction() {
  QKeyEvent* event = widget->action;
  switch (event->key()) {
    case Qt::Key_Up:
      return UserAction_t::Up;
    case Qt::Key_Down:
      return UserAction_t::Down;
    case Qt::Key_Left:
      return UserAction_t::Left;
    case Qt::Key_Right:
      return UserAction_t::Right;
    default:
      return UserAction_t::No_Action;
  }
  widget->action = No_Action;
};
void UpdateState(s21::Frontend_Interface* model) {
  this->widget->setMatrix(model->getMatrix());
};