#include "brick_game.h"

#include "../globals.h"

static GameInfo_t* currentGame;
static void get_score(int rows);
static void game_loop(UserAction_t action);
static matrix_t process_array(int rows, int cols, int arr[rows][cols]);
static const char* game_state_to_string(GAME_STATE_t state);
static double bg_get_speed();
static figure_t* prepare_figures();
figure_t* get_figure() {
  static figure_t* figures = NULL;
  if (figures == NULL) {
    figures = prepare_figures();
  }
  return figures;
}

GameInfo_t updateCurrentState() { return *currentGame; }
void initCurrentState() { currentGame = bg_init(); }

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  game_loop(action);
}

void accelerate_figure(figure_t* figure, matrix_t* fixed_figure) {
  while (!check_collision(figure, fixed_figure)) {
    figure->y++;
  }
}

int check_collision(figure_t* figure, matrix_t* fixed_figure) {
  // Defensive checks to avoid NULL dereferences
  if (figure == NULL || fixed_figure == NULL) return 1;
  if (figure->figure.matrix == NULL || fixed_figure->matrix == NULL) return 1;
  // Проход по всем рядам и столбцам фигуры
  int frows = figure->figure.rows;
  int fcols = figure->figure.columns;
  if (frows <= 0 || fcols <= 0) return 1;
  for (int r = 0; r < frows; r++) {
    for (int c = 0; c < fcols; c++) {
      if (figure->figure.matrix[r][c] != 1)
        continue;                  // учитываем только заполненные клетки фигуры
      int by = figure->y + r + 1;  // клетка под текущей
      int bx = figure->x + c;

      // Столкновение с полом
      if (by >= fixed_figure->rows) {
        s21_sum_figure(figure, fixed_figure);
        validate_fixed_figure(fixed_figure);
        return 1;
      }

      // Пропускаем проверку вне верхней границы
      if (by < 0) continue;

      // Горизонтальные границы
      if (bx < 0 || bx >= fixed_figure->columns) continue;

      // Столкновение с зафиксированными блоками
      if (fixed_figure->matrix[by][bx] == 1) {
        s21_sum_figure(figure, fixed_figure);
        validate_fixed_figure(fixed_figure);
        return 1;
      }
    }
  }
  return 0;
}

int s21_sum_figure(figure_t* figure, matrix_t* dst) {
  if (!figure || !dst || !figure->figure.matrix || !dst->matrix) return 2;

  for (int i = 0; i < figure->figure.rows; i++) {
    for (int j = 0; j < figure->figure.columns; j++) {
      int dy = figure->y + i;
      int dx = figure->x + j;
      if (dy < 0 || dy >= dst->rows || dx < 0 || dx >= dst->columns) continue;

      int cell = figure->figure.matrix[i][j];
      if (cell == 1) {
        if (dst->matrix[dy][dx] == 1) {
          return 1;
        }
        dst->matrix[dy][dx] = 1;
      }
    }
  }
  return 0;
}
void s21_sum_matrices(matrix_t* src, matrix_t* dst) {
  if (dst == NULL || src == NULL || src->rows != dst->rows ||
      src->columns != dst->columns) {
    return;
  }
  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->columns; j++) {
      dst->matrix[i][j] = src->matrix[i][j] == 1 ? 1 : dst->matrix[i][j];
    }
  }
}

int drop_figure() {
  static struct timeval last_drop = {0};
  struct timeval now;
  gettimeofday(&now, NULL);
  double speed = bg_get_speed();
  long drop_interval = (long)(speed * 1000000);

  // Таймер падения фигуры
  if (now.tv_sec * 1000000 + now.tv_usec -
          (last_drop.tv_sec * 1000000 + last_drop.tv_usec) >
      drop_interval) {
    last_drop = now;
    return 1;
  }
  return 0;
}

void move_figure(figure_t* figure, matrix_t* screen, matrix_t* fixed_figure,
                 int direction) {
  int rows = figure->figure.rows;
  int columns = figure->figure.columns;
  int y = figure->y;

  // Try horizontal movement here and revert if collides with fixed blocks
  if (direction != 0) {
    figure->x += direction;
    // clamp to screen bounds
    if (figure->x + columns > screen->columns) {
      figure->x = screen->columns - columns;
    }
    if (figure->x < 0) {
      figure->x = 0;
    }

    matrix_t temp;
    s21_clone_matrix(fixed_figure, &temp);
    if (s21_sum_figure(figure, &temp)) {
      figure->x -= direction;
    }
    s21_remove_matrix(&temp);
  }

  int x = figure->x;
  for (int i = 0; i < rows; i++) {
    if (i + y >= 0 && i + y < screen->rows) {
      for (int z = 0; z < columns; z++) {
        if (x + z >= 0 && x + z < screen->columns) {
          screen->matrix[i + y][x + z] = figure->figure.matrix[i][z];
        }
      }
    }
  }
}

void validate_fixed_figure(matrix_t* fixed_figure) {
  int rows_cleared = 0;
  int full_rows[20];
  for (int f = 0; f < 20; ++f) full_rows[f] = -1;
  for (int i = 0; i < fixed_figure->rows; i++) {
    int full_row = 1;
    for (int j = 0; j < fixed_figure->columns; j++) {
      if (fixed_figure->matrix[i][j] == 0) {
        full_row = 0;
        break;
      }
    }
    if (full_row) {
      full_rows[rows_cleared] = i;
      rows_cleared += 1;
      for (int k = 0; k < fixed_figure->columns; k++) {
        fixed_figure->matrix[i][k] = 0;
      }
    }
  }
  if (rows_cleared > 0) {
    drop_fixed_figure(fixed_figure, full_rows);
    get_score(rows_cleared);
  }
}

void drop_fixed_figure(matrix_t* fixed_figure, const int full_rows[20]) {
  matrix_t new_matrix;
  s21_create_matrix(fixed_figure->rows, fixed_figure->columns, &new_matrix);
  int write_y = fixed_figure->rows - 1;
  for (int read_y = fixed_figure->rows - 1; read_y >= 0; --read_y) {
    int is_full = 0;
    for (int k = 0; k < 20; ++k) {
      if (full_rows[k] == -1) break;
      if (full_rows[k] == read_y) {
        is_full = 1;
        break;
      }
    }
    if (!is_full) {
      for (int x = 0; x < fixed_figure->columns; ++x) {
        new_matrix.matrix[write_y][x] = fixed_figure->matrix[read_y][x];
      }
      write_y--;
    }
  }
  for (int y = write_y; y >= 0; --y) {
    for (int x = 0; x < fixed_figure->columns; ++x) {
      new_matrix.matrix[y][x] = 0;
    }
  }
  s21_remove_matrix(fixed_figure);
  fixed_figure->matrix = new_matrix.matrix;
  fixed_figure->rows = new_matrix.rows;
  fixed_figure->columns = new_matrix.columns;
}

void rotate_figure(figure_t* figure, matrix_t* fixed_figure) {
  int rows = figure->figure.rows;
  int columns = figure->figure.columns;
  matrix_t new_matrix = {0};
  s21_create_matrix(columns, rows, &new_matrix);
  for (int i = 0; i < columns; i++) {
    for (int j = 0; j < rows; j++) {
      new_matrix.matrix[i][j] = figure->figure.matrix[j][columns - i - 1];
    }
  }
  figure_t candidate;
  candidate.rotation = figure->rotation;
  candidate.x = figure->x;
  candidate.y = figure->y;
  candidate.figure = new_matrix;
  if (candidate.x + candidate.figure.columns > fixed_figure->columns ||
      candidate.y + candidate.figure.rows >= fixed_figure->rows) {
    s21_remove_matrix(&new_matrix);
    return;
  }
  matrix_t temp;
  s21_create_matrix(fixed_figure->rows, fixed_figure->columns, &temp);
  s21_sum_matrices(fixed_figure, &temp);
  int result = s21_sum_figure(&candidate, &temp);
  s21_remove_matrix(&temp);
  if (result == 1) {
    s21_remove_matrix(&new_matrix);
    return;
  }
  s21_remove_matrix(&figure->figure);
  figure->figure = new_matrix;
}

figure_t* prepare_figures() {
  figure_t* figures = malloc(8 * sizeof(figure_t));
  // I
  int i_matrix[1][4] = {{1, 1, 1, 1}};

  // O
  int o_matrix[2][2] = {{1, 1}, {1, 1}};

  // T
  int t_matrix[2][3] = {{0, 1, 0}, {1, 1, 1}};

  // L
  int l_matrix[3][2] = {{1, 0}, {1, 0}, {1, 1}};

  // J
  int j_matrix[3][2] = {{0, 1}, {0, 1}, {1, 1}};

  // S
  int s_matrix[2][3] = {{0, 1, 1}, {1, 1, 0}};

  // Z
  int z_matrix[2][3] = {{1, 1, 0}, {0, 1, 1}};
  figures[0] = (figure_t){3, 0, create_figure_matrix(1, 4, i_matrix), 0};
  figures[1] = (figure_t){3, -1, create_figure_matrix(2, 2, o_matrix), 0};
  figures[2] = (figure_t){3, -1, create_figure_matrix(2, 3, t_matrix), 0};
  figures[3] = (figure_t){3, -1, create_figure_matrix(3, 2, l_matrix), 0};
  figures[4] = (figure_t){3, -1, create_figure_matrix(3, 2, j_matrix), 0};
  figures[5] = (figure_t){3, -1, create_figure_matrix(2, 3, s_matrix), 0};
  figures[6] = (figure_t){3, -1, create_figure_matrix(2, 3, z_matrix), 0};
  figures[7] = (figure_t){0};  // сентинел
  return figures;
}

figure_t spawn_figure(const figure_t* figures, GameInfo_t* ctx) {
  // Count available figures until sentinel (figure.matrix == NULL)
  // Defensive upper bound to avoid reading past allocated pool
  int figure_num = rand() % 7;
  figure_t figure = figures[figure_num];
  figure_t new_figure = {0};
  if (s21_clone_matrix(&figure.figure, &new_figure.figure) != 0) {
    s21_create_matrix(0, 0, &new_figure.figure);
  }
  new_figure.x = figure.x;
  new_figure.y = figure.y;
  new_figure.rotation = figure.rotation;
  if (ctx != NULL) {
    figure_t current = ctx->next_figure;
    ctx->next_figure = new_figure;
    return current;
  } else {
    return new_figure;
  }
}

static void get_score(int rows) {
  int* score = &(currentGame->score);
  if (rows == 1) {
    (*score) += 100;
  } else if (rows == 2) {
    (*score) += 300;
  } else if (rows == 3) {
    (*score) += 700;
  } else if (rows == 4) {
    (*score) += 1500;
  }
  int* level = &(currentGame->level);
  *level = (int)(*score / 600);
  double* speed = &(currentGame->speed);
  *(speed) = 1 - (*level) * 0.1;
  save_score(*score);
}

void save_score(int score) {
  FILE* file = fopen("score.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  fprintf(file, "%d", score);
  fclose(file);
}

matrix_t create_figure_matrix(int rows, int columns, int mat[rows][columns]) {
  matrix_t a = process_array(rows, columns, mat);
  return a;
}

GameInfo_t* bg_init() {
  GameInfo_t* ctx = (GameInfo_t*)malloc(sizeof(GameInfo_t));
  if (!ctx) return NULL;

  // Инициализация всех полей нулями
  ctx->fixed_figure = NULL;
  ctx->screen = NULL;
  ctx->figures = NULL;
  ctx->next_figure.figure.matrix = NULL;
  ctx->STATE = ST_WAITING;
  ctx->score = 0;
  ctx->level = 0;
  ctx->speed = 1.0;
  ctx->hi_score = 0;

  // Выделяем память для матриц
  ctx->fixed_figure = (matrix_t*)malloc(sizeof(matrix_t));
  ctx->screen = (matrix_t*)malloc(sizeof(matrix_t));

  if (!ctx->fixed_figure || !ctx->screen) {
    if (ctx->fixed_figure) free(ctx->fixed_figure);
    if (ctx->screen) free(ctx->screen);
    free(ctx);
    return NULL;
  }

  // Инициализируем матрицы нулями
  ctx->fixed_figure->matrix = NULL;
  ctx->fixed_figure->rows = 0;
  ctx->fixed_figure->columns = 0;

  ctx->screen->matrix = NULL;
  ctx->screen->rows = 0;
  ctx->screen->columns = 0;

  // Создаем игровое поле (фиксированные блоки)
  if (s21_create_matrix(HEIGHT, WIDTH, ctx->fixed_figure) != SUCCESS) {
    free(ctx->fixed_figure);
    free(ctx->screen);
    free(ctx);
    return NULL;
  }

  // Создаем экран для отрисовки
  if (s21_create_matrix(HEIGHT, WIDTH, ctx->screen) != SUCCESS) {
    s21_remove_matrix(ctx->fixed_figure);
    free(ctx->fixed_figure);
    free(ctx->screen);
    free(ctx);
    return NULL;
  }

  // Инициализируем пул фигур
  ctx->figures = get_figure();
  if (!ctx->figures) {
    s21_remove_matrix(ctx->fixed_figure);
    s21_remove_matrix(ctx->screen);
    free(ctx->fixed_figure);
    free(ctx->screen);
    free(ctx);
    return NULL;
  }

  // Спавним первую фигуру
  ctx->next_figure = spawn_figure(ctx->figures, NULL);

  // Устанавливаем начальное состояние
  ctx->STATE = ST_WAITING;
  ctx->score = 0;
  ctx->level = 0;
  ctx->speed = 1.0;

  // Загружаем рекорд из файла
  FILE* file = fopen("score.txt", "r");
  if (file != NULL) {
    if (fscanf(file, "%d", &ctx->hi_score) != 1) {
      ctx->hi_score = 0;
    }
    fclose(file);
  } else {
    ctx->hi_score = 0;
  }
  return ctx;
}

void bg_free() {
  GameInfo_t* ctx = currentGame;
  if (!ctx) return;

  if (ctx->fixed_figure) {
    s21_remove_matrix(ctx->fixed_figure);
    free(ctx->fixed_figure);
  }

  if (ctx->screen) {
    s21_remove_matrix(ctx->screen);
    free(ctx->screen);
  }

  if (ctx->figures) {
    for (int i = 0; i < 7; i++) {
      if (ctx->figures[i].figure.matrix) {
        s21_remove_matrix(&ctx->figures[i].figure);
      }
    }
    free(ctx->figures);
  }

  s21_remove_matrix(&ctx->next_figure.figure);
  free(ctx);
  currentGame = NULL;
}

// figure_t* bg_get_figures() { return currentGame->figures; }
figure_t bg_next_figure() { return currentGame->next_figure; }
matrix_t* bg_get_fixed() { return currentGame->fixed_figure; }
// static int bg_get_score() { return currentGame->score; }
static int bg_get_level() { return currentGame->level; }
static double bg_get_speed() { return currentGame->speed; };
static int bg_get_state() { return currentGame->STATE; }
void bg_set_state(int state) {
  if (currentGame->STATE != state) {
    // write_log("%s\n", game_state_to_string(state));
  }
  currentGame->STATE = state;
}

matrix_t* bg_get_screen() { return currentGame->screen; }

static void game_loop(UserAction_t action) {
  matrix_t* fixed_figure = bg_get_fixed();
  matrix_t* screen = bg_get_screen();
  int level = bg_get_level();
  int state = bg_get_state();
  static figure_t current;
  static int has_piece = 0;

  if (level > 10) state = ST_WIN;
  for (int i = 0; i < fixed_figure->columns; ++i) {
    if (fixed_figure->matrix[0][i] == 1) {
      bg_set_state(ST_OVER);
      // Ensure switch operates on the updated state
      state = ST_OVER;
      break;
    }
  }
  if (action == Terminate) {
    bg_set_state(ST_OVER);
    // Ensure switch operates on the updated state
    state = ST_OVER;
  }

  switch (state) {
    case ST_WAITING:
      if (action == Start) {
        bg_set_state(ST_SPAWN);
      }
      break;
    case ST_PAUSE:
      if (action == Pause) {
        bg_set_state(ST_FALLING);
      }
      break;
    case ST_WIN:
      if (has_piece) {
        s21_remove_matrix(&current.figure);
        has_piece = 0;
      }
      return;
    case ST_OVER:
      if (has_piece) {
        s21_remove_matrix(&current.figure);
        has_piece = 0;
      }
      break;
    case ST_SPAWN: {
      const figure_t* pool = get_figure();
      if (!pool) return;
      current = spawn_figure(pool, currentGame);
      has_piece = 1;
      bg_set_state(ST_FALLING);
      break;
    }
    case ST_FALLING: {
      if (!has_piece || current.figure.matrix == NULL || fixed_figure == NULL ||
          fixed_figure->matrix == NULL) {
        bg_set_state(ST_SPAWN);
        break;
      }
      if (drop_figure()) {
        (&current)->y++;
      }
      if (check_collision(&current, fixed_figure)) {
        bg_set_state(ST_LOCK);
        break;
      }
      if (action == Pause) {
        bg_set_state(ST_PAUSE);
        break;
      }
      int dir = 0;
      if (action == Left) dir = -1;
      if (action == Right) dir = 1;
      if (dir != 0) {
        current.x += dir;
        if (current.x < 0) current.x = 0;
        if (current.x + current.figure.columns > fixed_figure->columns) {
          current.x = fixed_figure->columns - current.figure.columns;
        }
        matrix_t temp;
        s21_clone_matrix(fixed_figure, &temp);
        if (s21_sum_figure(&current, &temp)) {
          current.x -= dir;  // Revert movement if collision detected
        }
        s21_remove_matrix(&temp);
      }
      if (action == Action) {
        rotate_figure(&current, fixed_figure);
      }
      if (action == Down) {
        accelerate_figure(&current, fixed_figure);
        bg_set_state(ST_LOCK);
      }
      // Clear screen each tick to avoid ghosting
      if (screen && screen->matrix) {
        for (int r = 0; r < screen->rows; ++r) {
          for (int c = 0; c < screen->columns; ++c) {
            screen->matrix[r][c] = 0;
          }
        }
      }
      move_figure(&current, screen, fixed_figure, 0);
      s21_sum_matrices(fixed_figure, screen);
      break;
    }
    case ST_LOCK:
      bg_set_state(ST_CLEAR);
      break;
    case ST_CLEAR:
      if (has_piece) {
        s21_remove_matrix(&current.figure);
        has_piece = 0;
      }
      bg_set_state(ST_SPAWN);
      break;
    default:
      break;
  }
}

static matrix_t process_array(int rows, int cols, int arr[rows][cols]) {
  matrix_t a;
  s21_create_matrix(rows, cols, &a);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      a.matrix[i][j] = arr[i][j];
    }
  }
  return a;
}

void bg_free_test() {
  GameInfo_t* ctx = currentGame;
  if (!ctx) return;

  // 1. Освобождаем матрицы внутри структур
  if (ctx->fixed_figure) {
    s21_remove_matrix(ctx->fixed_figure);  // Освобождаем данные матрицы
    free(ctx->fixed_figure);               // Освобождаем саму структуру
    ctx->fixed_figure = NULL;
  }

  if (ctx->screen) {
    s21_remove_matrix(ctx->screen);
    free(ctx->screen);
    ctx->screen = NULL;
  }

  // 2. Освобождаем матрицы в фигурах
  if (ctx->next_figure.figure.matrix) {
    s21_remove_matrix(&ctx->next_figure.figure);
  }

  if (ctx->next_figure.figure.matrix) {
    s21_remove_matrix(&ctx->next_figure.figure);
  }

  // 4. Освобождаем основную структуру
  free(ctx);
  currentGame = NULL;
}
