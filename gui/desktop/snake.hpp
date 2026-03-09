#pragma once
#include <ncurses.h>

#include <map>

#include "../../brick_game/models/render.h"

namespace s21 {

template <size_t height, size_t width, int start_y, int start_x>
class Snake_Render : public s21::Render {
 private:
  enum State { ST_WAITING, ST_MOVE, ST_PAUSE, ST_WIN, ST_LOSE };
  typedef bool (Snake_Render::*InputHandler)(int);
  typedef void (Snake_Render::*ScreenHandler)();
  const int HEIGHT = height;
  const int WIDTH = width;
  const int START_Y = start_y;
  const int START_X = start_x;
  State cur_state;
  WINDOW *window;
  std::map<State, InputHandler> input_handlers = {
      {ST_WAITING, &Snake_Render::Waiting_Handler},
      {ST_MOVE, &Snake_Render::Move_Handler},
      {ST_PAUSE, &Snake_Render::Pause_Handler},
      {ST_WIN, &Snake_Render::Win_Handler},
      {ST_LOSE, &Snake_Render::Lose_Handler}};
  std::map<State, ScreenHandler> screen_handlers = {
      {ST_WAITING, &Snake_Render::WaitingScreen_Handler},
      {ST_MOVE, &Snake_Render::MoveScreen_Handler},
      {ST_PAUSE, &Snake_Render::PauseScreen_Handler},
      {ST_WIN, &Snake_Render::WinScreen_Handler},
      {ST_LOSE, &Snake_Render::LoseScreen_Handler}};

  bool Waiting_Handler(int key) {
    if (key == 's') {
      this->cur_state = ST_MOVE;
      return true;
    }
    return 0;
  }

  bool Move_Handler(int key) {
    if (key == 'p') {
      this->cur_state = ST_PAUSE;
      return true;
    }
    return 0;
  }

  bool Pause_Handler(int key) {
    if (key == 'p') {
      this->cur_state = ST_MOVE;
      return true;
    }
    return 0;
  }

  bool Win_Handler(int key) { return 0; }

  bool Lose_Handler(int key) { return 0; }

  void WaitingScreen_Handler() { mvwprintw(this->window, 2, 2, "WAITING"); }

  void MoveScreen_Handler() { mvwprintw(this->window, 2, 2, "MOVE"); }

  void PauseScreen_Handler() { mvwprintw(this->window, 2, 2, "PAUSE"); }

  void WinScreen_Handler() { mvwprintw(this->window, 2, 2, "WIN"); }

  void LoseScreen_Handler() { mvwprintw(this->window, 2, 2, "LOSE"); }

 public:
  Snake_Render(s21::GameModel *mdl) {
    this->model = mdl;
    this->cur_state = ST_WAITING;
    this->window =
        newwin(this->HEIGHT, this->WIDTH, this->START_Y, this->START_X);
    if (initscr() == nullptr) {
      cbreak();               // отключаем буферизацию строк
      noecho();               // не отображать вводимые символы
      keypad(stdscr, TRUE);   // включаем функциональные клавиши
      nodelay(stdscr, TRUE);  // неблокирующий ввод (опционально)
    }
  };

  InputHandler GetInputHandler() { return input_handlers[this->cur_state]; }

  UserAction_t GetAction() override {
    int ch = getch();
    UserAction_t action = No_Action;

    InputHandler handle = this->GetInputHandler();
    if (handle && (this->*handle)(ch)) {
      return action;
    }  // если клавиша перехвачена интерфесом - игнорируем

    if (ch != ERR) {
      switch (ch) {
        case 's':
          action = Start;
          break;
        case 'q':
          action = Terminate;
          break;
        case KEY_LEFT:
          action = Left;
          break;
        case KEY_RIGHT:
          action = Right;
          break;
        case KEY_DOWN:
          action = Down;
          break;
        case ' ':
          action = Action;
          break;
        case 'p':
          action = Pause;
          break;
      }
    }

    return action;
  }

  void UpdateState(s21::GameModel *model) override {
    this->model = model;
    ScreenHandler drawF = this->screen_handlers[this->cur_state];
    if (drawF) {
      (this->*drawF)();
      wrefresh(this->window);
    }
  }

  ~Snake_Render() override {
    delwin(this->window);
    endwin();
  }
};

}  // namespace s21
