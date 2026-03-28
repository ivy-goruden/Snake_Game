#include <cstddef>

#include "../../../brick_game/models/controller.h"
#include "snake_desktop_render.h"
namespace s21 {
class QT_Snake_Controller : public QObject, public Controller {
  Q_OBJECT
 private:
  s21::Snake_Game *model;
  s21::Snake_Desktop_Render *render;

 private slots:
  void onKeyPressed(UserAction_t key);

 public:
  QT_Snake_Controller(Snake_Game *model, Snake_Desktop_Render *render,
                      QObject *parent = nullptr);
  void Run() override;
  bool Tick() override;
  void onTick();
};
}  // namespace s21