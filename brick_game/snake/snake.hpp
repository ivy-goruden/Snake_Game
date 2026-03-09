#include <array>
#include <list>
#include "../globals.h"
#include "../models/gameModel.h"

namespace s21 {

template <size_t width, size_t height>
class Snake_Game : public GameModel {
 private:
  int lenght;
  int WIDTH = width;
  int HEIGHT = height; 
  std::list<Position> &body;
  int direction;
  Position apple;
  bool ateApple;

  typedef enum {
    ST_MOVE,
    ST_OVER,
    ST_EATING,
    SIZE,
  } State;

  typedef bool (*Trigger)(); //функция триггер для перехода между состояниями FSM
  typedef void (*Handler)(UserAction_t); //функция триггер для перехода между состояниями FSM

  std::array<std::array<Trigger, SIZE>, SIZE> FSM_Triggers;
  std::array<Handler, SIZE> FSM_Handlers;
  State cur_state;

  void Move_Handler(UserAction_t);
  void Over_Handler(UserAction_t);
  void Eating_Handler(UserAction_t);
  bool FoundApple();
  bool Smashed();
  void IsWin();
  void isOver();
  void Forward();
  void NewApple();


 public:
  int LEFT = 0;
  int UP = 1;
  int RIGHT = 2;
  int DOWN = 3;


  Snake_Game<width, height>() {
    this->cur_state = ST_MOVE;
    this->FSM_Triggers = {
      // ST_MOVE  ST_OVER     ST_EATING
      {no(),      isOver(),   FoundApple()}, //   ST_MOVE
      {no(),      no(),       no()},         //   ST_OVER
      {yes(),     no(),       no()}          //   ST_EATING
    };
    this->FSM_Handlers = {
      Move_Handler(),
      Over_Handler(),
      Eating_Handler()
    };
  };
};
}  // namespace S21