#include "pixeltextwidget.h"

#include <QKeyEvent>
#include <QPainter>

PixelTextWidget::PixelTextWidget(QWidget* parent)
    : QWidget(parent), m_pixelSize(12) {}

void PixelTextWidget::setMatrix(const QVector<QVector<int>>& matrix) {
  m_matrix = matrix;
  updateGeometry();
  update();
}

int PixelTextWidget::pixelSize() const { return m_pixelSize; }

void PixelTextWidget::setPixelSize(int size) {
  if (size < 1) size = 1;
  if (m_pixelSize == size) return;
  m_pixelSize = size;
  updateGeometry();
  update();
}

QColor PixelTextWidget::onColor() const { return m_on; }

void PixelTextWidget::setOnColor(const QColor& c) {
  if (m_on == c) return;
  m_on = c;
  update();
}

QColor PixelTextWidget::offColor() const { return m_off; }

void PixelTextWidget::setOffColor(const QColor& c) {
  if (m_off == c) return;
  m_off = c;
  update();
}

QSize PixelTextWidget::sizeHint() const {
  if (m_matrix.isEmpty()) return {200, 100};
  int rows = m_matrix.size();
  int cols = 0;
  for (const auto& r : m_matrix) cols = qMax(cols, r.size());
  return {cols * m_pixelSize, rows * m_pixelSize};
}

void PixelTextWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event)
  QPainter p(this);
  p.fillRect(rect(), palette().window());
  if (m_matrix.isEmpty()) return;
  int rows = m_matrix.size();
  int cols = 0;
  for (const auto& r : m_matrix) cols = qMax(cols, r.size());
  for (int y = 0; y < rows; ++y) {
    const auto& row = m_matrix[y];
    for (int x = 0; x < cols; ++x) {
      bool on = x < row.size() ? row[x] != 0 : false;
      p.fillRect(x * m_pixelSize, y * m_pixelSize, m_pixelSize - 1,
                 m_pixelSize - 1, on ? m_on : m_off);
    }
  }
}

void PixelTextWidget::keyPressEvent(QKeyEvent* event) { this->action = event; }
