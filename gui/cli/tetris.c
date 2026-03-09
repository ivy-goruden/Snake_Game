#include "game.h"

#include <locale.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../brick_game/tetris/tetris.h"

void init_log() {
  FILE* file = fopen("log.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  fclose(file);
}

void write_log(const char* fmt, ...) {
  FILE* file = fopen("log.txt", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(file, fmt, args);
  fputc('\n', file);
  va_end(args);
  fclose(file);
}

// Helpers to render different parts of the UI
static void draw_top_border(const matrix_t screen) {
  mvaddstr(0, 0, DOUBLE_TOP_LEFT);
  for (int i = 0; i < screen.columns; i++) {
    int x1 = 1 + i * 2;
    int x2 = 2 + i * 2;
    mvaddstr(0, x1, DOUBLE_HORIZONTAL);
    mvaddstr(0, x2, DOUBLE_HORIZONTAL);
  }
  int trx = 1 + screen.columns * 2;
  mvaddstr(0, trx, DOUBLE_TOP_RIGHT);
}

static void draw_rows_and_cells(const matrix_t screen) {
  for (int i = 0; i < screen.rows; i++) {
    mvaddstr(i + 1, 0, DOUBLE_VERTICAL);
    for (int j = 0; j < screen.columns; j++) {
      char cell = screen.matrix ? screen.matrix[i][j] : ' ';
      int x1 = 1 + j * 2;
      int x2 = 2 + j * 2;
      if (cell == 1) {
        mvaddstr(i + 1, x1, BLOCK);
        mvaddstr(i + 1, x2, BLOCK);
      } else {
        mvaddstr(i + 1, x1, " ");
        mvaddstr(i + 1, x2, " ");
      }
    }
    int xr = 1 + screen.columns * 2;
    mvaddstr(i + 1, xr, DOUBLE_VERTICAL);
  }
}

static void draw_bottom_border(const matrix_t screen) {
  int by = screen.rows + 1;
  mvaddstr(by, 0, DOUBLE_BOTTOM_LEFT);
  for (int i = 0; i < screen.columns; i++) {
    int x1 = 1 + i * 2;
    int x2 = 2 + i * 2;
    mvaddstr(by, x1, DOUBLE_HORIZONTAL);
    mvaddstr(by, x2, DOUBLE_HORIZONTAL);
  }
  int brx = 1 + screen.columns * 2;
  mvaddstr(by, brx, DOUBLE_BOTTOM_RIGHT);
}

static void draw_stats(int stats_x, int score, int level, int hi_score) {
  mvprintw(1, stats_x, "Score: %d", score);
  mvprintw(2, stats_x, "Level: %d", level);
  mvprintw(3, stats_x, "High Score: %d", hi_score);
}

static void draw_next_box_and_content(int stats_x, figure_t next_figure) {
  int nf_rows = next_figure.figure.rows;
  int nf_cols = next_figure.figure.columns;
  double** nf_matrix = next_figure.figure.matrix;
  if (nf_rows > 0 && nf_cols > 0 && nf_matrix != NULL) {
    for (int i = 0; i < nf_rows; i++) {
      for (int x = 0; x < nf_cols; x++) {
        if (nf_matrix[i][x] == 1.0) {
          int yy = 5 + i;
          int xx = stats_x + 1 + x;
          mvaddstr(yy, xx, BLOCK);
        }
      }
    }
    for (int x = 0; x < nf_cols + 2; x++) {
      int xx = stats_x + x;
      int top_y = 4;
      int bot_y = 4 + nf_rows + 1;
      mvaddstr(top_y, xx, CAGE);
      mvaddstr(bot_y, xx, CAGE);
    }
    for (int i = 0; i < nf_rows; i++) {
      int yy = 5 + i;
      int lx = stats_x;
      int rx = stats_x + nf_cols + 1;
      mvaddstr(yy, lx, CAGE);
      mvaddstr(yy, rx, CAGE);
    }
  }
}

static void draw_controls_footer(int start_y) {
  int y = start_y + 1;
  mvprintw(y, 1, "Управление:");
  mvprintw(y + 1, 1, "s — старт, q — выход, p — пауза");
  mvprintw(y + 2, 1, "←/→ — движение, ↓ — ускорить, пробел — действие");
}

static int print_screen(matrix_t screen, figure_t next_figure, int score,
                        int level, int hi_score) {
  erase();
  int max_y = 0, max_x = 0;
  getmaxyx(stdscr, max_y, max_x);

  // Требуемая минимальная ширина/высота: поле + панель справа + футер
  int nf_cols = next_figure.figure.columns;
  if (nf_cols < 0) nf_cols = 0;
  int required_w_board = 2 * screen.columns + 2;
  int required_w_total =
      required_w_board + 2 + nf_cols + 2;  // stats_x + box width
  int required_h = screen.rows + 4;        // низ поля + 3 строки футера
  if (max_x < required_w_total || max_y < required_h) {
    erase();
    mvprintw(max_y / 2, 1, "Окно слишком маленькое. Требуется минимум %d×%d",
             required_w_total, required_h);
    return 0;
  }

  draw_top_border(screen);
  draw_rows_and_cells(screen);
  draw_bottom_border(screen);

  int stats_x = 1 + screen.columns * 2 + 2;
  if (score > hi_score) hi_score = score;
  draw_stats(stats_x, score, level, hi_score);
  draw_next_box_and_content(stats_x, next_figure);

  int by = screen.rows + 1;
  draw_controls_footer(by);
  return 0;
}

static int key_event(int ch, UserAction_t action) {
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
    default:
      break;
  }
  userInput(action, 0);
  return 1;
}

int main() {
  init_log();
  setlocale(LC_ALL, "");
  initscr();  // Initialize the screen and ncurses environment
  cbreak();   // Disable line buffering, making input immediately available
  noecho();   // Turn off echoing of user input to the screen
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  initCurrentState();
  GameInfo_t bg = updateCurrentState();
  UserAction_t action = -1;
  int state = bg.STATE;
  int hi_score = bg.hi_score;
  const matrix_t* screen = bg_get_screen();
  while (state != ST_WIN && state != ST_OVER) {
    int ch = getch();
    key_event(ch, action);
    bg = updateCurrentState();
    int score = bg.score;
    int level = bg.level;
    state = bg.STATE;
    print_screen(*screen, bg_next_figure(), score, level, hi_score);
    refresh();
    usleep(20000);
  }
  endwin();
  if (state == ST_WIN) {
    printf("%s\n", "You Win!");
  } else {
    printf("%s\n", "Game Over");
  }
  bg_free();
  return 0;
}