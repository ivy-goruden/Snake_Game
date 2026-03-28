#include <QGraphicsRectItem>
#include <QScreen>

#include "build/Qt_6_10_2_for_macOS-Debug/Snake_autogen/include/ui_snake_desktop_render.h"
#include "snake_desktop_render.h"

void s21::Snake_Desktop_Render::WaitingScreen_Handler(QPainter *paint) {
  this->ui->tabWidget->show();
  this->ui->tabWidget->setCurrentIndex(0);
};
void s21::Snake_Desktop_Render::MoveScreen_Handler(QPainter *paint) {
  this->ui->tabWidget->hide();

  auto body = this->model->GetBody();
  paint->fillRect(rect(), Qt::black);

  int offset_y = 50;
  int cell_size = (this->W_HEIGHT - 2 * offset_y) / HEIGHT;
  int offset_x = (this->W_WIDTH - cell_size * 10) / 2;

  paint->setPen(Qt::gray);
  for (int i = 0; i <= WIDTH; ++i) {
    paint->drawLine(offset_x + i * cell_size, offset_y,
                    offset_x + i * cell_size, offset_y + HEIGHT * cell_size);
  }
  for (int i = 0; i <= HEIGHT; ++i) {
    paint->drawLine(offset_x, offset_y + i * cell_size,
                    offset_x + WIDTH * cell_size, offset_y + i * cell_size);
  }
  paint->setBrush(Qt::red);
  Position apple = this->model->GetApple();
  paint->drawRect(offset_x + apple.x * cell_size,
                  offset_y + apple.y * cell_size, cell_size, cell_size);

  for (auto i = body.begin(); i != body.end(); i++) {
    if (i == body.begin()) {
      paint->setBrush(Qt::darkGreen);
    } else {
      paint->setBrush(Qt::green);
    }
    paint->drawRect(offset_x + i->x * cell_size, offset_y + i->y * cell_size,
                    cell_size, cell_size);
  }

  this->ui->tabWidget->setCurrentIndex(1);
};
void s21::Snake_Desktop_Render::PauseScreen_Handler(QPainter *paint) {
  this->ui->tabWidget->show();
  this->ui->tabWidget->setCurrentIndex(2);
};
void s21::Snake_Desktop_Render::WinScreen_Handler(QPainter *paint) {
  this->ui->tabWidget->show();
  this->ui->tabWidget->setCurrentIndex(4);
};
void s21::Snake_Desktop_Render::LoseScreen_Handler(QPainter *paint) {
  this->ui->tabWidget->show();
  this->ui->tabWidget->setCurrentIndex(3);
};
