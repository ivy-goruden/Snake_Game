#ifndef BRICK_GAME
#define BRICK_GAME

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
} Position;

bool yes(){
  return true;
}

bool no(){
  return false;
}


#endif