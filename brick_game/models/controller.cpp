#include "controller.h"

using namespace s21;

void s21::Controller::Quit() { running = false; }

// чтобы убить ncurses если это не удастся
// родителям(Snake_Render, Tetris_Render)
void s21::Controller::TerminateHandler() {
  printf("Работает перехват ошибок\n");
  try {
    if (std::current_exception()) {
      std::rethrow_exception(std::current_exception());
    }
  } catch (const std::exception& e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception caught in terminate handler." << std::endl;
  }
}