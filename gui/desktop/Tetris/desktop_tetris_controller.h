#include <cstddef>

#include "../../../brick_game/models/controller.h"
#include "tetris_desktop_render.h"
namespace s21 {
class QT_Tetris_Controller : public QObject, public Controller {
  Q_OBJECT
 private:
  s21::Tetris_Game *model;
  s21::Tetris_Desktop_Render *render;

 private slots:
  void onKeyPressed(UserAction_t key);

 public:
  QT_Tetris_Controller(Tetris_Game *model, Tetris_Desktop_Render *render,
                       QObject *parent = nullptr);
  void Run() override;
  bool Tick() override;
  void onTick();
};
}  // namespace s21