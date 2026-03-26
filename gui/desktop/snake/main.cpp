#define NCURSES_NOMACROS
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <iostream>
#include <memory>

#include "../../../brick_game/globals.h"
#include "../../../brick_game/models/controller.h"
#include "../../../brick_game/snake/snake.hpp"
#include "../../../brick_game/tetris/tetris.hpp"
#include "../../cli/snake/snake.hpp"
#include "../../cli/tetris/tetris.hpp"
#include "controller.h"
#include "snake_desktop_render.h"

std::unique_ptr<s21::Controller> c;

int main(int argc, char** argv) {
  QApplication a(argc, argv);
  init_log();
  auto game = s21::Snake_Game();
  auto render = s21::Snake_Desktop_Render(&game);
  s21::QT_Snake_Controller c = s21::QT_Snake_Controller(&game, &render);
  c.Run();
  return a.exec();
  return 0;
}
