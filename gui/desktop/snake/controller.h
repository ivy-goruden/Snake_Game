#include <cstddef>

#include "../../../brick_game/models/controller.h"
#include "snake_desktop_render.h"
namespace s21 {
class QT_Snake_Controller : public Controller {
 private:
  s21::Snake_Game *model;
  s21::Snake_Desktop_Render *render;

 public:
  QT_Snake_Controller(s21::Snake_Game *model, s21::Snake_Desktop_Render *render)
      : Controller(nullptr, nullptr), model(model), render(render) {};
};
}  // namespace s21