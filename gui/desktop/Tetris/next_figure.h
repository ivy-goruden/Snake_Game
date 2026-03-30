#ifndef NEXT_FIGURE_H
#define NEXT_FIGURE_H
#include <QPainter>
#include <QWidget>

#include "../../../brick_game/tetris/frontend.hpp"
#include "build/Qt_6_10_2_for_macOS-Debug/Tetris_autogen/include/ui_next_figure.h"

namespace Ui {
class next_figure;
}

class next_figure : public QWidget {
  Q_OBJECT

 public:
  explicit next_figure(QWidget* parent = nullptr);
  ~next_figure();
  void set_Figure(const s21::matrix_t*);

 private:
  Ui::next_figure* ui;
  s21::matrix_t figure;
  int width_w = 100;
  int height_w = 100;
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
};

#endif  // NEXT_FIGURE_H
