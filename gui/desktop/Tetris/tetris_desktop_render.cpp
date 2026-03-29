#include "tetris_desktop_render.h"

#include <unistd.h>

#include <QString>
#include <QTabBar>
#include <QTextEdit>
#include <string>

#include "build/Tetris_autogen/include/ui_tetris_desktop_render.h"

using namespace s21;

Tetris_Desktop_Render::Tetris_Desktop_Render(TetrisFrontendData* mdl,
                                             QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Tetris_Desktop_Render) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  this->model = static_cast<TetrisFrontendData*>(mdl);
  this->cur_state = ST_WAITING;
  setFixedSize(W_WIDTH, W_HEIGHT);
  ui->tabWidget->tabBar()->hide();
  ui->level->setReadOnly(true);
  ui->hi_score->setReadOnly(true);
  ui->lenght->setReadOnly(true);
  show();
}

Tetris_Desktop_Render::~Tetris_Desktop_Render() { delete ui; }

UserAction_t Tetris_Desktop_Render::GetAction() { return No_Action; }

void Tetris_Desktop_Render::SendAction(UserAction_t action) {
  emit keyPressed(action);
}

void Tetris_Desktop_Render::UpdateState(Frontend_Interface* model) {
  this->model = static_cast<TetrisFrontendData*>(model);
  if (this->model->IsLose()) {
    this->cur_state = ST_LOSE;
  }
  if (this->model->IsWin()) {
    this->cur_state = ST_WIN;
  }
  this->Draw();
}

void Tetris_Desktop_Render::keyPressEvent(QKeyEvent* event) {
  int key = event->key();
  UserAction_t action = No_Action;
  InputHandler handler = getInputHandler();
  if (handler && (this->*handler)(key)) {
    this->Draw();
    return;
  }
  switch (key) {
    case Qt::Key_Q:
      action = Terminate;
      break;
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      action = Enter;
      break;
    default:
      break;
  }
  this->SendAction(action);
}

Tetris_Desktop_Render::InputHandler Tetris_Desktop_Render::getInputHandler() {
  return this->input_handlers[this->cur_state];
}

void Tetris_Desktop_Render::Draw() {
  update();
  Draw_Stats();
}

void Tetris_Desktop_Render::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  ScreenHandler handler = this->screen_handlers[this->cur_state];
  if (handler) {
    (this->*handler)(&painter);
  }
}

bool Tetris_Desktop_Render::isRunning() { return cur_state == ST_MOVE; }

void Tetris_Desktop_Render::on_tabWidget_currentChanged(int index) {}

void Tetris_Desktop_Render::Draw_Stats() {
  ui->level->setText(QString::number(int(this->model->GetLevel())));
  ui->hi_score->setText(QString::number(int(this->model->GetHighScore())));
  ui->lenght->setText(QString::number(int(this->model->GetBody().size())));
}