#include "main.h"

START_TEST(test1) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  s21_create_matrix(20, 10, &fixed);
  figures[0].y = 15;
  figures[0].x = 0;
  int res = check_collision(&figures[0], &fixed);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&fixed);
}
END_TEST

START_TEST(test2) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  s21_create_matrix(20, 10, &fixed);
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;

  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[0].figure, &figure.figure);
  figure.y = 19;
  figure.x = 9;
  s21_sum_figure(&figure, &fixed);
  figures[3].y = 18;
  figures[3].x = 9;
  int res = check_collision(&figures[3], &fixed);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&fixed);
  s21_remove_matrix(&figure.figure);
}
END_TEST

START_TEST(test3) {
  figure_t* figures = get_figure();
  matrix_t fixed;
  s21_create_matrix(20, 10, &fixed);
  figure_t figure;
  figure.figure.rows = 0;
  figure.figure.columns = 0;
  figure.figure.matrix = NULL;
  figure.rotation = 0;
  figure.x = 0;
  figure.y = 0;
  s21_clone_matrix(&figures[2].figure, &figure.figure);
  rotate_figure(&figure, &fixed);
  figure.y = 19;
  figure.x = 9;

  int res = check_collision(&figure, &fixed);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&fixed);
  s21_remove_matrix(&figure.figure);
}
END_TEST

Suite* check_collision_suite(void) {
  Suite* s = suite_create("check_collision_tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test1);
  tcase_add_test(tc_core, test2);
  tcase_add_test(tc_core, test3);
  suite_add_tcase(s, tc_core);
  return s;
}

int main_check_collision(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;
  s = check_collision_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
