#ifndef BRICK_GAME
#define BRICK_GAME

#define SUCCESS 0
#define INVALID_MATRIX 1
#define CALCULATION_ERROR 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../globals.h"
#include "s21_matrix.h"
typedef struct figure {
  int x;
  int y;
  matrix_t figure;
  int rotation;
} figure_t;

typedef enum {
  ST_WAITING,  // До начала игры
  ST_SPAWN,
  ST_WIN,
  ST_OVER,
  ST_PAUSE,
  ST_FALLING,
  ST_LOCK,
  ST_CLEAR
} GAME_STATE_t;
typedef struct {
  figure_t* figures;
  matrix_t* fixed_figure;
  matrix_t* screen;
  figure_t next_figure;
  GAME_STATE_t STATE;
  int score;
  int level;
  double speed;
  int hi_score;
} GameInfo_t;

GameInfo_t* bg_init();
void bg_free();
figure_t* bg_get_figures();
figure_t bg_next_figure();
matrix_t* bg_get_screen();
matrix_t* bg_get_fixed();
void save_score(int score);
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();
int s21_create_matrix(int rows, int columns, matrix_t* result);
void s21_remove_matrix(matrix_t* a);
int s21_clone_matrix(matrix_t* src, matrix_t* dst);
void move_figure(figure_t* figure, matrix_t* screen, matrix_t* fixed_figure,
                 int direction);
int check_collision(figure_t* figure, matrix_t* fixed_figure);
int s21_sum_figure(figure_t* figure, matrix_t* dst);
void s21_sum_matrices(matrix_t* src, matrix_t* dst);
void write_log(const char* fmt, ...);
void validate_fixed_figure(matrix_t* fixed_figure);
void rotate_figure(figure_t* figure, matrix_t* fixed_figure);
figure_t* get_figure();
matrix_t create_figure_matrix(int rows, int columns, int mat[rows][columns]);
void accelerate_figure(figure_t* figure, matrix_t* fixed_figure);
int drop_figure();
void drop_fixed_figure(matrix_t* fixed_figure, const int full_rows[20]);
GameInfo_t updateCurrentState();
void initCurrentState();
void userInput(UserAction_t action, bool hold);
void bg_set_state(int state);
void bg_free_test();
#endif