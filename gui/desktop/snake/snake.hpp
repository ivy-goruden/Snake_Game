#include "../../../brick_game/models/render.h"
#include "pixeltextwidget.h"
namespace s21 {

class Snake_Desktop_Render : public s21::Render {
 private:
  PixelTextWidget* widget;

 public:
  UserAction_t GetAction() override;
  void UpdateState(s21::Frontend_Interface* model) override;
};
}  // namespace s21