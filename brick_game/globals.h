#ifndef BRICK_GAME
#define BRICK_GAME

#include <generator>

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Tick
} UserAction_t;

typedef struct Position {
  int x;
  int y;

bool operator==(const Position& other) const {
  return x == other.x && y == other.y;
}

bool operator!=(const Position& other) const {
  return x != other.x || y != other.y;
}

Position operator=(const Position& other) {
  return Position{x: other.x, y: other.y};
}
} Position;

bool yes(){
  return true;
}

bool no(){
  return false;
}

template<typename T>
T random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}


#endif