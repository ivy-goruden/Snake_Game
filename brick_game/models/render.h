#include <iostream>
#include "../globals.h"
namespace S21 {

class Render {
 protected:
  GameModel *model;
 public:
  virtual UserAction_t GetAction();
  virtual void UpdateState(GameModel *model);
    
  virtual ~Render() = default;
};

}  // namespace S21
