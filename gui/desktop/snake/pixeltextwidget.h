#ifndef PIXELTEXTWIDGET_H
#define PIXELTEXTWIDGET_H

#include <QColor>
#include <QVector>
#include <QWidget>

class PixelTextWidget : public QWidget {
  Q_OBJECT
  Q_PROPERTY(int pixelSize READ pixelSize WRITE setPixelSize)
  Q_PROPERTY(QColor onColor READ onColor WRITE setOnColor)
  Q_PROPERTY(QColor offColor READ offColor WRITE setOffColor)
 public:
  explicit PixelTextWidget(QWidget* parent = nullptr);
  void setMatrix(const QVector<QVector<int>>& matrix);
  int pixelSize() const;
  void setPixelSize(int size);
  QColor onColor() const;
  void setOnColor(const QColor& c);
  QColor offColor() const;
  void setOffColor(const QColor& c);
  QSize sizeHint() const override;
  void keyPressEvent(QKeyEvent* event) override;
  QKeyEvent* action;

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QVector<QVector<int>> m_matrix;
  int m_pixelSize;
  QColor m_on{0, 0, 0};
  QColor m_off{220, 220, 220};
};

#endif  // PIXELTEXTWIDGET_H
