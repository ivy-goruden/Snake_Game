#include "main.h"

START_TEST(test1) {
  matrix_t a;
  matrix_t b;
  s21_create_matrix(10, 10, &a);
  s21_clone_matrix(&a, &b);
  int a1 = s21_eq_matrix(&a, &b);
  ck_assert_int_eq(a1, 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
}
END_TEST

// START_TEST(test2) {
//   matrix_t a;
//   matrix_t b;
//   s21_clone_matrix(&a, &b);
//   int a1 = s21_eq_matrix(&a, &b);
//   ck_assert_int_eq(a1, 0);
// }
// END_TEST

Suite* clone_suite(void) {
  Suite* s = suite_create("clone_tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test1);
  // tcase_add_test(tc_core, test2);
  suite_add_tcase(s, tc_core);
  return s;
}

// Запуск тестов
int main_clone(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;

  s = clone_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
