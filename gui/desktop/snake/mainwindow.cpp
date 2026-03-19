#include "mainwindow.h"

#include <QVector>

#include "./ui_mainwindow.h"
#include "pixeltextwidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  auto* w = new PixelTextWidget(this);
  QVector<QVector<int>> h = {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
                             {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
                             {1, 0, 0, 0, 1}};
  QVector<QVector<int>> i = {{1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
                             {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
                             {1, 1, 1, 1, 1}};
  int rows = 7;
  int space = 1;
  int cols = h[0].size() + space + i[0].size();
  QVector<QVector<int>> matrix(rows, QVector<int>(cols, 0));
  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < h[y].size(); ++x) matrix[y][x] = h[y][x];
    int offset = h[0].size() + space;
    for (int x = 0; x < i[y].size(); ++x) matrix[y][offset + x] = i[y][x];
  }
  w->setPixelSize(16);
  w->setOnColor(Qt::black);
  w->setOffColor(QColor(230, 230, 230));
  w->setMatrix(matrix);
  setCentralWidget(w);
}

MainWindow::~MainWindow() { delete ui; }
