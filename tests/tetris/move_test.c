#include "main.h"

START_TEST(test1) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  matrix_t dst;
  matrix_t screen;
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[2].figure, &figure.figure);
  figure.y = 0;
  figure.x = 0;
  s21_create_matrix(4, 4, &fixed);
  s21_create_matrix(4, 4, &screen);
  move_figure(&figure, &screen, &fixed, 0);
  int str_expected[4][4] = {
      {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dst.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&screen, &dst);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&screen);
  s21_remove_matrix(&fixed);
  s21_remove_matrix(&dst);
  s21_remove_matrix(&figure.figure);
}
END_TEST

START_TEST(test2) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  matrix_t dst;
  matrix_t screen;
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[2].figure, &figure.figure);
  figure_t figure2;
  figure2.figure.rows = 0;
  figure2.figure.columns = 0;
  figure2.figure.matrix = NULL;

  figure2.rotation = 0;
  figure2.x = 0;
  figure2.y = 0;
  s21_clone_matrix(&figures[0].figure, &figure2.figure);
  figure.y = 1;
  figure.x = 0;
  figure2.y = 3;
  figure2.x = 0;
  s21_create_matrix(4, 4, &fixed);
  s21_create_matrix(4, 4, &screen);
  s21_sum_figure(&figure2, &fixed);
  move_figure(&figure, &screen, &fixed, 1);
  int str_expected[4][4] = {
      {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dst.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&screen, &dst);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&screen);
  s21_remove_matrix(&fixed);
  s21_remove_matrix(&dst);
  s21_remove_matrix(&figure.figure);
  s21_remove_matrix(&figure2.figure);
}
END_TEST

START_TEST(test3) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  matrix_t dst;
  matrix_t screen;
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[1].figure, &figure.figure);
  figure_t figure2;
  figure2.figure.rows = 0;
  figure2.figure.columns = 0;
  figure2.figure.matrix = NULL;

  figure2.rotation = 0;
  figure2.x = 0;
  figure2.y = 0;
  s21_clone_matrix(&figures[2].figure, &figure2.figure);
  figure.y = 0;
  figure.x = 0;
  figure2.x = 1;
  figure2.y = 1;
  s21_create_matrix(4, 4, &fixed);
  s21_create_matrix(4, 4, &screen);
  s21_sum_figure(&figure2, &fixed);
  move_figure(&figure, &screen, &fixed, 1);
  int str_expected[4][4] = {
      {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dst.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&screen, &dst);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&fixed);
  s21_remove_matrix(&dst);
  s21_remove_matrix(&screen);
  s21_remove_matrix(&figure.figure);
  s21_remove_matrix(&figure2.figure);
}
END_TEST

Suite* move_figure_suite(void) {
  Suite* s = suite_create("move_figure_tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test1);
  tcase_add_test(tc_core, test2);
  tcase_add_test(tc_core, test3);
  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_move_figure(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = move_figure_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
