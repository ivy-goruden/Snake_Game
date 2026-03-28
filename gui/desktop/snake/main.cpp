#define NCURSES_NOMACROS
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>

#include "../../../brick_game/snake/snake.hpp"
#include "../../cli/tetris/tetris.hpp"
#include "desktop_snake_controller.h"
#include "snake_desktop_render.h"

int main(int argc, char** argv) {
  QApplication a(argc, argv);
  auto game = s21::Snake_Game();
  auto render = s21::Snake_Desktop_Render(&game);
  render.show();
  s21::QT_Snake_Controller controller(&game, &render);
  controller.Run();
  return a.exec();
}