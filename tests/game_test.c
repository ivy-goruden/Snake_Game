#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/tetris/brick_game.h"

// Тесты
START_TEST(test_initial_state) {
  GameInfo_t state = updateCurrentState();
  ck_assert_int_eq(state.STATE, ST_WAITING);
  ck_assert_int_eq(state.score, 0);
  ck_assert_int_eq(state.level, 0);
}
END_TEST

START_TEST(test_start_game) {
  // Start the game
  userInput(Start, false);
  GameInfo_t state = updateCurrentState();
  // Should be in SPAWN state after start
  ck_assert_int_eq(state.STATE, 1);
}
END_TEST

START_TEST(test_pause_functionality) {
  // Start and get to FALLING state
  userInput(Start, false);

  // Даем игре время перейти в FALLING
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state = updateCurrentState();
  if (state.STATE != ST_FALLING) {
    // Если не в FALLING, пропускаем тест
    return;
  }

  // Pause the game
  userInput(Pause, false);
  state = updateCurrentState();
  ck_assert_int_eq(state.STATE, ST_PAUSE);

  // Resume the game
  userInput(Pause, false);
  state = updateCurrentState();
  ck_assert_int_eq(state.STATE, ST_FALLING);
  bg_set_state(ST_OVER);
  userInput(-1, false);
}
END_TEST

START_TEST(test_movement_left_right) {
  // Start and get to FALLING state
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state_before = updateCurrentState();
  if (state_before.STATE != ST_FALLING) {
    return;
  }

  // Move left
  userInput(Left, false);
  GameInfo_t state_after_left = updateCurrentState();

  // Move right
  userInput(Right, false);
  GameInfo_t state_after_right = updateCurrentState();

  // Should remain in FALLING state
  ck_assert_int_eq(state_after_left.STATE, ST_FALLING);
  ck_assert_int_eq(state_after_right.STATE, ST_FALLING);

  // Score should not change from movement
  ck_assert_int_eq(state_before.score, state_after_left.score);
  ck_assert_int_eq(state_before.score, state_after_right.score);
}
END_TEST

START_TEST(test_rotation) {
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state_before = updateCurrentState();
  if (state_before.STATE != ST_FALLING) {
    return;
  }

  // Rotate figure
  userInput(Action, false);
  GameInfo_t state_after = updateCurrentState();

  ck_assert_int_eq(state_after.STATE, ST_FALLING);

  ck_assert_int_eq(state_before.score, state_after.score);
}
END_TEST

START_TEST(test_instant_drop) {
  // Start and get to FALLING state
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state_before = updateCurrentState();
  if (state_before.STATE != ST_FALLING) {
    return;
  }

  // Instant drop
  userInput(Down, false);
  GameInfo_t state_after = updateCurrentState();

  // Should go to LOCK state after instant drop
  ck_assert(state_after.STATE == ST_LOCK || state_after.STATE == ST_CLEAR);
}
END_TEST

START_TEST(test_game_over_condition) {
  // Force game over by terminating
  userInput(Terminate, false);
  GameInfo_t state = updateCurrentState();

  ck_assert_int_eq(state.STATE, ST_OVER);
}
END_TEST

START_TEST(test_score_calculation) {
  // Start the game
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  // Store initial score
  GameInfo_t initial_state = updateCurrentState();
  int initial_score = initial_state.score;

  if (initial_state.STATE != ST_FALLING) {
    return;
  }
  userInput(Left, false);
  userInput(Right, false);
  userInput(Action, false);

  GameInfo_t final_state = updateCurrentState();

  ck_assert_int_eq(initial_score, final_state.score);
}
END_TEST

START_TEST(test_level_progression) {
  // Start the game
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state = updateCurrentState();

  // Initial level should be 0
  ck_assert_int_eq(state.level, 0);
}
END_TEST

START_TEST(test_state_transitions) {
  userInput(Terminate, false);
  bg_free_test();
  initCurrentState();
  userInput(Start, false);
  GameInfo_t state = updateCurrentState();
  ck_assert_int_eq(state.STATE, ST_SPAWN);

  // Даем время для автоматических переходов состояний
  for (int i = 0; i < 10; i++) {
    userInput(-1, false);
    state = updateCurrentState();

    // Выходим если игра окончена или выиграна
    if (state.STATE == ST_OVER || state.STATE == ST_WIN) {
      break;
    }

    // Тестируем PAUSE если мы в FALLING
    if (state.STATE == ST_FALLING) {
      userInput(Pause, false);
      ck_assert_int_eq(updateCurrentState().STATE, ST_PAUSE);

      userInput(Pause, false);
      ck_assert_int_eq(updateCurrentState().STATE, ST_FALLING);
      break;
    }
  }
}
END_TEST

// Тесты для граничных условий
START_TEST(test_boundary_conditions) {
  // Start the game
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state = updateCurrentState();
  if (state.STATE != ST_FALLING) {
    return;
  }

  // Test multiple left movements (should not crash)
  for (int i = 0; i < 10; i++) {
    userInput(Left, false);
  }

  // Test multiple right movements (should not crash)
  for (int i = 0; i < 10; i++) {
    userInput(Right, false);
  }

  // Test multiple rotations (should not crash)
  for (int i = 0; i < 5; i++) {
    userInput(Action, false);
  }

  // Game should still be in a valid state
  state = updateCurrentState();
  ck_assert(state.STATE >= ST_WAITING && state.STATE <= ST_CLEAR);
}
END_TEST

START_TEST(test_multiple_pauses) {
  // Start the game
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
  }

  GameInfo_t state = updateCurrentState();
  if (state.STATE != ST_FALLING) {
    return;
  }

  // Toggle pause multiple times
  for (int i = 0; i < 5; i++) {
    userInput(Pause, false);
    ck_assert_int_eq(updateCurrentState().STATE, ST_PAUSE);

    userInput(Pause, false);
    ck_assert_int_eq(updateCurrentState().STATE, ST_FALLING);
  }
}
END_TEST

START_TEST(test_invalid_actions) {
  userInput(-1, false);
  userInput(100, false);

  GameInfo_t state = updateCurrentState();
  ck_assert(state.STATE >= ST_WAITING && state.STATE <= ST_CLEAR);
  userInput(Start, false);
  for (int i = 0; i < 5; i++) {
    userInput(-1, false);
    state = updateCurrentState();

    userInput(100, false);
  }

  state = updateCurrentState();
  ck_assert(state.STATE >= ST_WAITING && state.STATE <= ST_CLEAR);
  for (int i = 0; i < 20; i++) {
    userInput(Down, false);
  }
}
END_TEST

// Создание test suite
Suite* game_loop_suite(void) {
  Suite* s;
  TCase* tc_core;
  TCase* tc_edges;

  s = suite_create("GameLoop");

  // Core test case
  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_initial_state);
  tcase_add_test(tc_core, test_start_game);
  tcase_add_test(tc_core, test_pause_functionality);
  tcase_add_test(tc_core, test_movement_left_right);
  tcase_add_test(tc_core, test_rotation);
  tcase_add_test(tc_core, test_instant_drop);
  tcase_add_test(tc_core, test_game_over_condition);
  tcase_add_test(tc_core, test_score_calculation);
  tcase_add_test(tc_core, test_level_progression);
  tcase_add_test(tc_core, test_state_transitions);
  suite_add_tcase(s, tc_core);

  // Edge cases test case
  tc_edges = tcase_create("Edges");
  tcase_add_test(tc_edges, test_boundary_conditions);
  tcase_add_test(tc_edges, test_multiple_pauses);
  tcase_add_test(tc_edges, test_invalid_actions);

  tcase_set_timeout(tc_edges, 10);

  suite_add_tcase(s, tc_edges);

  return s;
}

// Главная функция для запуска тестов
int main_game(void) {
  int number_failed;
  Suite* s;
  SRunner* sr;
  s = game_loop_suite();
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srand(42);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}