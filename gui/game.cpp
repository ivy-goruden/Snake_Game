
#include <stdio.h>
#include <stdlib.h>

#include <cstddef>
#include <cstdio>

#include "brick_game/models/controller.h"
#include "brick_game/snake/snake.hpp"
#include "gui/desktop/snake.hpp"
int main() {
  init_log();
  int game;
  // s21::Controller c = s21::Controller(nullptr, nullptr);
  while (true) {
    printf("Choose game:\n");
    printf("    1.Tetris\n");
    printf("    2.Snake\n");
    scanf("%d", &game);
    switch (game) {
      case 1:
        printf("bad choice\n");
        break;
      case 2:
        auto game = s21::Snake_Game<10, 20>();
        auto render = s21::Snake_Render<15, 20, 0, 0>(&game);
        auto c = s21::Controller(&game, &render);
        c.Run();
    }
  }
  return 0;
}