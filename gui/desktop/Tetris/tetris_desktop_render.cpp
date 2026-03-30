#include "tetris_desktop_render.h"

#include <unistd.h>

#include <QLayout>
#include <QString>
#include <QTabBar>
#include <QTextEdit>

#include "build/Qt_6_10_2_for_macOS-Debug/Tetris_autogen/include/ui_next_figure.h"
#include "next_figure.h"
using namespace s21;

Tetris_Desktop_Render::Tetris_Desktop_Render(TetrisFrontendData* mdl,
                                             QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::Tetris_Desktop_Render),
      nextFigure(nullptr) {
  ui->setupUi(this);
  setFocusPolicy(Qt::StrongFocus);
  this->model = static_cast<TetrisFrontendData*>(mdl);
  this->cur_state = ST_WAITING;
  setFixedSize(W_WIDTH, W_HEIGHT);

  configure_widgets();

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
  Draw_Stats();
  update();
}

void Tetris_Desktop_Render::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  
  // Fill background
  painter.fillRect(this->rect(), Qt::black);
  
  ScreenHandler handler = this->screen_handlers[this->cur_state];
  if (handler) {
    qDebug() << "Calling handler, state: " << this->cur_state;
    (this->*handler)(&painter);
  }
}

bool Tetris_Desktop_Render::isRunning() { return cur_state == ST_MOVE; }

void Tetris_Desktop_Render::on_tabWidget_currentChanged(int index) {}

void Tetris_Desktop_Render::Draw_Stats() {
  ui->level->setText(QString::number(int(this->model->GetLevel())));
  ui->hi_score->setText(QString::number(int(this->model->GetHighScore())));
  ui->score->setText(QString::number(int(this->model->GetScore())));
  if (this->nextFigure != nullptr) {
    auto nFigure = this->model->GetNextFigure();
    this->nextFigure->set_Figure(&nFigure);
  }
}

void Tetris_Desktop_Render::set_FigureWidget(next_figure* f) {
  this->nextFigure = f;
}

void Tetris_Desktop_Render::configure_text(QTextEdit* textEdit) {
  textEdit->setReadOnly(true);
  textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

}

void Tetris_Desktop_Render::configure_widgets() {
  configure_text(ui->level);
  configure_text(ui->hi_score);
  configure_text(ui->score);
  configure_text(ui->level_h);
  configure_text(ui->hi_score_h);
  configure_text(ui->score_h);
  ui->tabWidget->tabBar()->hide();
  // Create next_figure widget
  next_figure* nextFigureWidget = new next_figure(this);
  this->nextFigure = nextFigureWidget;

  // Try to replace in layout if it exists
  QWidget* nextFigureContainer = ui->nextFigure;
  QLayout* layout = nextFigureContainer->parentWidget()->layout();
  if (layout) {
    int index = layout->indexOf(nextFigureContainer);
    layout->replaceWidget(nextFigureContainer, nextFigureWidget);
    delete nextFigureContainer;
    ui->nextFigure = nextFigureWidget;
  } else {
    // If no layout, just hide the original and use geometry
    nextFigureWidget->setGeometry(nextFigureContainer->geometry());
    nextFigureWidget->setParent(nextFigureContainer->parentWidget());
    nextFigureContainer->hide();
  }

}