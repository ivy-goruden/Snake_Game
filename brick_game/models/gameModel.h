#include <iostream>
namespace S21 {

class GameModel {
  protected:
    long timer;

 public:
  GameModel* updateCurrentState(UserAction_t);
  long GetTimer(){
    return this->timer;
  };
  virtual ~GameModel() = default;
};
}  // namespace S21
