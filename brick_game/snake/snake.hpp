#include <map>
#include "../globals.h"

namespace S21 {

template <size_t width, size_t height>
class Snake_Game : public GameModel {
 private:
  int lenght;
  int WIDTH = width;
  int HEIGHT = height; 
  std::array<Position, width*height> body;
  int direction;

  typedef enum {
    ST_MOVE,
    ST_OVER,
    ST_EATING,
    SIZE,
  } State;

  typedef bool (*Trigger)(); //функция триггер для перехода между состояниями FSM
  typedef void (*Handler)(UserAction_t); //функция триггер для перехода между состояниями FSM

  std::array<State, std::array<Trigger, SIZE>, SIZE> FSM_Triggers;
  std::array<Handler, SIZE> FSM_Handlers;
  State cur_state;

  void Move_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void Eating_Handler(UserAction_t);

 public:
  int LEFT = 0;
  int UP = 1;
  int RIGHT = 2;
  int DOWN = 3;
  void addSegment() {};
  void move() {};
  bool checkSelfCollision() { return false; };
  bool checkWallCollision() { return false; };
  bool checkAppleCollision() { return false; };
  void setDirection(int dir) { direction = dir; };
  void IsWin() {};

  Snake_Game<width, height>() {
    this->cur_state = ST_WAITING;
    this->FSM_Triggers = {
      // ST_MOVE  ST_OVER ST_EATING
      {no(),      null,   null}, //   ST_MOVE
      {null,      no(),   null}, //   ST_OVER
      {null,      null,   no()}  //   ST_EATING
    };
    this->FSM_Handlers = {
      Move_Handler(),
      Over_Handler(),
      Eating_Handler()
    }
  };
};
}  // namespace S21