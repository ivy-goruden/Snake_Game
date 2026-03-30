#include <QApplication>
#include <QCoreApplication>

#include "../../../brick_game/tetris/tetris.hpp"
#include "desktop_tetris_controller.h"
#include "tetris_desktop_render.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  auto game = s21::Tetris_Game();
  auto render = s21::Tetris_Desktop_Render(&game);
  render.show();
  s21::QT_Tetris_Controller controller(&game, &render);
  controller.Run();
  return a.exec();
}
