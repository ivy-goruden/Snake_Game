#include <ncurses.h>
#include <unistd.h>

#include <cstring>
#include <map>

#include "../../../brick_game/globals.h"
#include "../../../brick_game/models/render.h"
#include "../../../brick_game/tetris/frontend.hpp"

namespace s21 {

class Tetris_Render : public s21::Render {
 private:
  s21::TetrisFrontendData* model;
  enum State { ST_WAITING, ST_MOVE, ST_PAUSE, ST_WIN, ST_LOSE };
  typedef bool (Tetris_Render::*InputHandler)(int);
  typedef void (Tetris_Render::*ScreenHandler)();
  int height;
  int width;
  int start_y;
  int start_x;
  State cur_state;
  WINDOW* window;
  WINDOW* stats_window;
  std::map<State, InputHandler> input_handlers = {
      {ST_WAITING, &Tetris_Render::Waiting_Handler},
      {ST_MOVE, &Tetris_Render::Move_Handler},
      {ST_PAUSE, &Tetris_Render::Pause_Handler},
      {ST_WIN, &Tetris_Render::Win_Handler},
      {ST_LOSE, &Tetris_Render::Lose_Handler}};
  std::map<State, ScreenHandler> screen_handlers = {
      {ST_WAITING, &Tetris_Render::WaitingScreen_Handler},
      {ST_MOVE, &Tetris_Render::MoveScreen_Handler},
      {ST_PAUSE, &Tetris_Render::PauseScreen_Handler},
      {ST_WIN, &Tetris_Render::WinScreen_Handler},
      {ST_LOSE, &Tetris_Render::LoseScreen_Handler}};

  bool Waiting_Handler(int);
  bool Move_Handler(int);
  bool Pause_Handler(int);
  bool Win_Handler(int);
  bool Lose_Handler(int);

  void WaitingScreen_Handler();
  void MoveScreen_Handler();
  void PauseScreen_Handler();
  void WinScreen_Handler();
  void LoseScreen_Handler();

 public:
  Tetris_Render(s21::TetrisFrontendData* mdl, int width = 10, int height = 20,
                int start_y = 0, int start_x = 0) {
    this->model = mdl;
    this->width = width;
    this->height = height;
    this->start_y = start_y;
    this->start_x = start_x;
    this->cur_state = ST_WAITING;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    this->window =
        newwin(this->height + 2, this->width + 2, this->start_y, this->start_x);
    this->stats_window = newwin(this->height + 2, 20, this->start_y,
                                this->start_x + this->width + 3);
    keypad(this->window, TRUE);
    keypad(this->stats_window, TRUE);
    keypad(stdscr, TRUE);
  };

  InputHandler GetInputHandler() { return input_handlers[this->cur_state]; }

  UserAction_t GetAction() override {
    usleep(20000);
    int ch = getch();
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
        case KEY_UP:
          action = Up;
          break;
        case ' ':
          action = Action;
          break;
        case KEY_ENTER:
          action = Enter;
          write_log("Enter is sent from gui");
          break;
        case '\n':
          action = Enter;
          write_log("Enter is sent from gui");
          break;
      }
    }

    return action;
  }

  void UpdateState(s21::Frontend_Interface* model) override {
    this->model = static_cast<s21::TetrisFrontendData*>(model);
    if (this->model->IsWin()) {
      ChangeState(ST_WIN);
    }
    if (this->model->IsLose()) {
      ChangeState(ST_LOSE);
    }
    Draw();
  }

  void Draw() {
    ScreenHandler drawF = this->screen_handlers[this->cur_state];
    if (drawF) {
      wclear(this->window);
      (this->*drawF)();
      box(this->window, 0, 0);
      wrefresh(this->window);
      Draw_Stats();
    }
  }

  void Draw_Stats() {
    wclear(this->stats_window);
    box(this->stats_window, 0, 0);
    mvwprintw(this->stats_window, 1, 2, "Score: %d", this->model->GetScore());
    mvwprintw(this->stats_window, 3, 2, "High Score: %d",
              this->model->GetHighScore());
    mvwprintw(this->stats_window, 5, 2, "Level: %d", this->model->GetLevel());
    matrix_t nextF = this->model->GetNextFigure();
    int indexY = 0;
    for (auto row : nextF) {
      int indexX = 0;
      for (auto val : row) {
        if (val == 1) {
          mvwprintw(this->stats_window, 7 + indexY, 2 + indexX, "#", val);
        }
        indexX++;
      }
      indexY++;
    }
    wrefresh(this->stats_window);
  }

  void ChangeState(State state) { this->cur_state = state; }

  ~Tetris_Render() override {
    delwin(this->window);
    delwin(this->stats_window);
    endwin();
  }
};
}  // namespace s21