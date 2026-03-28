#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include "../brick_game/globals.h"
#include "../brick_game/snake/snake.hpp"
#include "../brick_game/tetris/tetris.hpp"
#include "cli/cli_controller.h"
#include "cli/snake/snake.hpp"
#include "cli/tetris/tetris.hpp"

std::unique_ptr<s21::Controller> c;

void TerminateHandler() {
  c->TerminateHandler();
  c.reset();
  std::abort();
}

void SignalHandler(int signum) {
  if (signum == SIGINT) {
    c.reset();
    std::exit(signum);  // exit вызовет деструкторы глобальных объектов
  }
}

int main() {
  init_log();
  std::set_terminate(TerminateHandler);
  signal(SIGINT, SignalHandler);
  int choice;
  while (true) {
    printf("Choose game:\n");
    printf("    1.Tetris\n");
    printf("    2.Snake\n");
    if (scanf("%d", &choice) != 1) break;
    std::unique_ptr<s21::GameModel> game;  // Умный указатель на базовый класс
    std::unique_ptr<s21::Render> render;   // Умный указатель на базовый класс
    switch (choice) {
      case 1: {
        game = std::make_unique<s21::Tetris_Game>();
        render = std::make_unique<s21::Tetris_Render>(
            static_cast<s21::Tetris_Game*>(game.get()));
        break;
      }
      case 2: {
        game = std::make_unique<s21::Snake_Game>();
        render = std::make_unique<s21::Snake_Render>(
            static_cast<s21::Snake_Game*>(game.get()));
        break;
      }
      default:
        return 0;
    }
    c = std::make_unique<s21::CLI_Controller>(std::move(game),
                                              std::move(render));
    c->Run();
  }
  return 0;
}
