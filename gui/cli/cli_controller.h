#include "../../brick_game/models/controller.h"
namespace s21 {
class CLI_Controller : public Controller {
 protected:
  std::unique_ptr<s21::GameModel> model;
  std::unique_ptr<Render> render;

 public:
  CLI_Controller(std::unique_ptr<GameModel> model,
                 std::unique_ptr<Render> render)
      : Controller() {
    this->model = std::move(model);
    this->render = std::move(render);
  }
  void Run() override;
  bool Tick() override;
};
}  // namespace s21
