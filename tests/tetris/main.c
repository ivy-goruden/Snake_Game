#include "main.h"

int main() {
  initCurrentState();
  main_rotate();
  main_clone();
  main_check_collision();
  main_sum_figure();
  main_sum_matrix();
  main_move_figure();
  main_drop();
  main_game();
  main_validate_matrix();
  userInput(Terminate, false);
  bg_free();
  return 0;
}