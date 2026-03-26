#include <QGraphicsRectItem>

#include "./ui_snake_desktop_render.h"
#include "snake_desktop_render.h"

void s21::Snake_Desktop_Render::WaitingScreen_Handler() {
  this->ui->tabWidget->setCurrentIndex(0);
};
void s21::Snake_Desktop_Render::MoveScreen_Handler() {
  //   auto segments = this->model->GetBody();
  //   for (auto i = segments.begin(); i != segments.end(); i++) {
  //     QGraphicsRectItem* item =
  //         new QGraphicsRectItem(i->x * 20,  // размер клетки 20px
  //                               i->y * 20, 20, 20);

  //     if (i == segments.begin()) {
  //       item->setBrush(Qt::green);  // голова
  //     } else {
  //       item->setBrush(Qt::darkGreen);  // тело
  //     }
  //     addItem(item);}

  this->ui->tabWidget->setCurrentIndex(1);
};
void s21::Snake_Desktop_Render::PauseScreen_Handler() {
  this->ui->tabWidget->setCurrentIndex(2);
};
void s21::Snake_Desktop_Render::WinScreen_Handler() {
  this->ui->tabWidget->setCurrentIndex(4);
};
void s21::Snake_Desktop_Render::LoseScreen_Handler() {
  this->ui->tabWidget->setCurrentIndex(3);
};
