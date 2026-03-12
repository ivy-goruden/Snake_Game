#pragma once
#include <ncurses.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <map>

#include "brick_game/globals.h"
#include "brick_game/models/render.h"
#include "brick_game/snake/frontend.hpp"

namespace s21 {

class Snake_Render : public s21::Render {
 private:
  s21::SnakeFrontendData* model;
  enum State { ST_WAITING, ST_MOVE, ST_PAUSE, ST_WIN, ST_LOSE };
  typedef bool (Snake_Render::*InputHandler)(int);
  typedef void (Snake_Render::*ScreenHandler)();
  int HEIGHT;
  int WIDTH;
  int START_Y;
  int START_X;
  State cur_state;
  WINDOW* window;
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
      ChangeState(ST_MOVE);
    }
    if (key != ERR && key != 'q') {
      return true;  // ничего не делаем, если нажата другая клавиша
    }
    return false;
  }

  bool Move_Handler(int key) {
    if (key == 'p') {
      ChangeState(ST_PAUSE);
      return true;
    }
    return 0;
  }

  bool Pause_Handler(int key) {
    if (key == 'p') {
      ChangeState(ST_MOVE);
    }
    if (key != ERR && key != 'q') {
      return true;  // ничего не делаем, если нажата другая клавиша
    }
    return false;
  }

  bool Win_Handler(int key) {
    if (key == KEY_ENTER) {
      ChangeState(ST_WAITING);
    }
    if (key != ERR && key != 'q') {
      return true;  // ничего не делаем, если нажата другая клавиша
    }
    return false;
  }

  bool Lose_Handler(int key) {
    if (key == KEY_ENTER) {
      ChangeState(ST_WAITING);
    }
    if (key != ERR && key != 'q') {
      return true;  // ничего не делаем, если нажата другая клавиша
    }
    return false;
  }

  void WaitingScreen_Handler() {
    const char* msg = "PRESS S";
    const char* msg2 = "TO START ";
    int x = (this->WIDTH - sizeof(msg) + 1) / 2;
    int y = this->HEIGHT / 2;
    mvwprintw(this->window, y, x, msg);
    x = (this->WIDTH - sizeof(msg2) + 1) / 2;
    mvwprintw(this->window, y + 1, x, msg2);
  }

  void MoveScreen_Handler() {
    int rowsPerSeg = this->HEIGHT / this->model->Height();
    int colsPerSeg = this->WIDTH / this->model->Width();
    for (const auto& pos : this->model->GetBody()) {
      for (int i = 0; i < rowsPerSeg; ++i) {
        for (int j = 0; j < colsPerSeg; ++j) {
          mvwprintw(this->window, pos.y * rowsPerSeg + i,
                    pos.x * colsPerSeg + j, "O");
        }
      }
    }
  }

  void PauseScreen_Handler() { mvwprintw(this->window, 2, 2, "PAUSE  "); }

  void WinScreen_Handler() { mvwprintw(this->window, 2, 2, "WIN    "); }

  void LoseScreen_Handler() { mvwprintw(this->window, 2, 2, "LOSE   "); }

 public:
  Snake_Render(s21::SnakeFrontendData* mdl, int width = 10, int height = 20,
               int start_y = 0, int start_x = 0) {
    this->model = mdl;
    this->WIDTH = width;
    this->HEIGHT = height;
    this->START_Y = start_y;
    this->START_X = start_x;
    this->cur_state = ST_WAITING;
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    this->window =
        newwin(this->HEIGHT, this->WIDTH, this->START_Y, this->START_X);
  };

  InputHandler GetInputHandler() { return input_handlers[this->cur_state]; }

  UserAction_t GetAction() override {
    usleep(20000);
    int ch = getch();
    if (ch != ERR) {
      write_log("ch = %d", ch);
    }
    UserAction_t action = No_Action;

    InputHandler handle = this->GetInputHandler();
    if (handle && (this->*handle)(ch)) {
      Draw();  // перерисовываем экран после обработки ввода
      return action;
    }  // если клавиша перехвачена интерфесом - игнорируем

    if (ch != ERR) {
      switch (ch) {
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
      }
    }

    return action;
  }

  void UpdateState(s21::Frontend_Interface* model) override {
    this->model = static_cast<s21::SnakeFrontendData*>(model);
    Draw();
  }

  void Draw() {
    ScreenHandler drawF = this->screen_handlers[this->cur_state];
    if (drawF) {
      (this->*drawF)();
      box(this->window, 0, 0);
      wrefresh(this->window);
      write_log("RUNNING draw function");
    }
  }

  void ChangeState(State state) { this->cur_state = state; }

  ~Snake_Render() override {
    delwin(this->window);
    endwin();
  }
};

}  // namespace s21
