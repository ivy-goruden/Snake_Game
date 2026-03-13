#ifndef BRICK_GAME
#define BRICK_GAME

#include <iostream>
#include <random>

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

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Position& other) const {
    return x != other.x || y != other.y;
  }
} Position;

// Snake game defaults
const int WIDTH = 10;
const int HEIGHT = 20;

template <typename T>
T random(T range_from, T range_to) {
  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_int_distribution<T> distr(range_from, range_to);
  return distr(generator);
}

void init_log();
void write_log(const char* fmt, ...);

#endif
