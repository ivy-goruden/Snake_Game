#include "main.h"

START_TEST(test1) {
  figure_t* figures = get_figure();
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[0].figure, &figure.figure);
  matrix_t a;
  s21_create_matrix(10, 10, &a);
  rotate_figure(&figure, &a);
  matrix_t expected;
  s21_create_matrix(4, 1, &expected);
  expected.matrix[0][0] = 1;
  expected.matrix[1][0] = 1;
  expected.matrix[2][0] = 1;
  expected.matrix[3][0] = 1;

  int a1 = s21_eq_matrix(&figure.figure, &expected);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&figure.figure);
}
END_TEST

START_TEST(test2) {
  figure_t* figures = get_figure();
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[1].figure, &figure.figure);

  matrix_t a;
  s21_create_matrix(10, 10, &a);
  rotate_figure(&figure, &a);
  int str_expected[2][2] = {{1, 1}, {1, 1}};
  matrix_t expected;
  s21_create_matrix(2, 2, &expected);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      expected.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&figure.figure, &expected);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&figure.figure);
}
END_TEST

START_TEST(test_3) {
  figure_t* figures = get_figure();
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[2].figure, &figure.figure);
  matrix_t a;
  s21_create_matrix(10, 10, &a);
  rotate_figure(&figure, &a);
  int str_expected[3][2] = {{0, 1}, {1, 1}, {0, 1}};
  matrix_t expected;
  s21_create_matrix(3, 2, &expected);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      expected.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&figure.figure, &expected);
  s21_remove_matrix(&a);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&figure.figure);
  ck_assert_int_eq(a1, 1);
}
END_TEST

Suite* drop_suite(void) {
  Suite* s = suite_create("drop_tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test1);
  tcase_add_test(tc_core, test2);
  tcase_add_test(tc_core, test_3);
  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_drop(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = drop_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
