#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define INVALID_MATRIX 1
#define CALCULATION_ERROR 2

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t* result);

void s21_remove_matrix(matrix_t* A);

#define YES 1
#define NO 0

int s21_eq_matrix(matrix_t* A, matrix_t* B);

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result);
int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result);

int s21_mult_number(matrix_t* A, double number, matrix_t* result);
int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result);

int s21_transpose(matrix_t* A, matrix_t* result);
int s21_calc_complements(matrix_t* A, matrix_t* result);

double get_matrix_minor(matrix_t* a, int row, int column);
int real_determinant(matrix_t* a, double* result);
int s21_determinant(matrix_t* A, double* result);
int s21_inverse_matrix(matrix_t* A, matrix_t* result);

void print_matrix(matrix_t matrix);
void duplicate_matrix(matrix_t* src, matrix_t* dst);

int s21_clone_matrix(matrix_t* src, matrix_t* dst);
#endif  // SRC_S21_MATRIX_H_