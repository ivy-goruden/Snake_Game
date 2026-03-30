#include "next_figure.h"

#include <vector>

next_figure::next_figure(QWidget* parent)
    : QWidget(parent), ui(new Ui::next_figure) {
  ui->setupUi(this);
}

next_figure::~next_figure() { delete ui; }
void next_figure::set_Figure(const s21::matrix_t* f) {
  if (f == nullptr || f->empty()) {
    this->figure.clear();
    return;
  }
  this->figure.clear();
  try {
    for (const auto& row : *f) {
      this->figure.push_back(row);
    }
  } catch (const std::exception& e) {
    this->figure.clear();
    return;
  }
}
void next_figure::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setBrush(Qt::white);
  int cell_size = 30;
  if (figure.empty()) {
    return;  // Don't try to draw empty figure
  }
  for (int i = 0; i < figure.size(); i++) {
    for (int x = 0; x < figure[0].size(); x++) {
      if (this->figure[i][x]) {
        painter.drawRect(x * cell_size, i * cell_size, cell_size, cell_size);
      }
    }
  }
}

void next_figure::resizeEvent(QResizeEvent* event)
{
    this->width_w = width();
    this->height_w = height();
}