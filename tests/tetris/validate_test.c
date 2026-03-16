#include "main.h"

START_TEST(test1) {
  matrix_t screen;
  matrix_t dst;
  int str[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &screen);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      screen.matrix[i][j] = str[i][j];
    }
  }
  validate_fixed_figure(&screen);
  int str_expected[4][4] = {
      {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dst.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&screen, &dst);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&screen);
  s21_remove_matrix(&dst);
}
END_TEST

START_TEST(test2) {
  matrix_t screen;
  matrix_t dst;
  int str[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &screen);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      screen.matrix[i][j] = str[i][j];
    }
  }
  validate_fixed_figure(&screen);
  int str_expected[4][4] = {
      {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  s21_create_matrix(4, 4, &dst);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dst.matrix[i][j] = str_expected[i][j];
    }
  }
  int a1 = s21_eq_matrix(&screen, &dst);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&screen);
  s21_remove_matrix(&dst);
}
END_TEST

Suite* validate_matrix_suite(void) {
  Suite* s = suite_create("validate_matrix_tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test1);
  tcase_add_test(tc_core, test2);
  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_validate_matrix(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = validate_matrix_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
