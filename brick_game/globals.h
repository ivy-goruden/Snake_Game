#ifndef BRICK_GAME
#define BRICK_GAME

#ifdef __cplusplus
#include <iostream>
#include <random>
#endif

typedef enum {
  No_Action,
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Tick,
  Enter,
} UserAction_t;

typedef enum { LEFT, UP, RIGHT, DOWN } Direction;
typedef struct Position {
  int x;
  int y;

#ifdef __cplusplus
  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Position& other) const {
    return x != other.x || y != other.y;
  }
#endif
} Position;

// Snake game defaults
#define WIDTH 10
#define HEIGHT 20

#ifdef __cplusplus
template <typename T>
T random(T range_from, T range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<T> distr(range_from, range_to);
  return distr(generator);
}
#endif

void init_log();
void write_log(const char* fmt, ...);

#endif
