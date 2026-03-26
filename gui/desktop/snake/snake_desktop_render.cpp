#include "snake_desktop_render.h"

#include <unistd.h>

#include "./ui_snake_desktop_render.h"

s21::Snake_Desktop_Render::Snake_Desktop_Render(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Snake_Desktop_Render), action(No_Action) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
}

s21::Snake_Desktop_Render::Snake_Desktop_Render(s21::SnakeFrontendData* mdl,
                                                QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Snake_Desktop_Render), action(No_Action) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  this->model = static_cast<s21::SnakeFrontendData*>(mdl);
  this->cur_state = ST_WAITING;
}

s21::Snake_Desktop_Render::~Snake_Desktop_Render() { delete ui; }

UserAction_t s21::Snake_Desktop_Render::GetAction() {
  usleep(20000);

  UserAction_t current_action = this->action;
  InputHandler handler = getInputHandler();
  this->action = No_Action;
  if (handler && (this->*handler)(current_action)) {
    this->Draw();
    return No_Action;
  }
  return current_action;
}

void s21::Snake_Desktop_Render::UpdateState(s21::Frontend_Interface* model) {
  this->model = static_cast<s21::SnakeFrontendData*>(model);
  this->Draw();
}

void s21::Snake_Desktop_Render::keyPressEvent(QKeyEvent* event) {
  int key = event->key();

  switch (key) {
    case Qt::Key_Q:
      this->action = Terminate;
      break;
    case Qt::Key_Left:
      this->action = Left;
      break;
    case Qt::Key_Right:
      this->action = Right;
      break;
    case Qt::Key_Down:
      this->action = Down;
      break;
    case Qt::Key_Up:
      this->action = Up;
      break;
    case Qt::Key_Space:
      this->action = Action;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      this->action = Enter;
      break;
    default:
      break;
  }
}

s21::Snake_Desktop_Render::InputHandler
s21::Snake_Desktop_Render::getInputHandler() {
  return this->input_handlers[this->cur_state];
}

void s21::Snake_Desktop_Render::Draw() {
  ScreenHandler handler = this->screen_handlers[this->cur_state];
  if (handler) {
    (this->*handler)();
  }
}

void s21::Snake_Desktop_Render::on_tabWidget_currentChanged(int index) {}
