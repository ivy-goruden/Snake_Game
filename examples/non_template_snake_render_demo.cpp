#include <cstdio>
#include <list>

struct Position {
  int x;
  int y;
  bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

class ISnakeFrontendData {
 public:
  virtual int Width() const = 0;
  virtual int Height() const = 0;
  virtual std::list<Position> GetBody() const = 0;
  virtual ~ISnakeFrontendData() = default;
};

class ISnakeModel {
 public:
  virtual ISnakeFrontendData* updateCurrentState(int action) = 0;
  virtual ~ISnakeModel() = default;
};

template <int W, int H>
class SnakeGame : public ISnakeModel, public ISnakeFrontendData {
  std::list<Position> body;

 public:
  SnakeGame() {
    body.push_back({W / 2, H / 2});
    body.push_back({W / 2 - 1, H / 2});
  }
  int Width() const override { return W; }
  int Height() const override { return H; }
  std::list<Position> GetBody() const override { return body; }
  ISnakeFrontendData* updateCurrentState(int) override { return this; }
};

class SnakeRender {
  ISnakeFrontendData* model;
  int renderW;
  int renderH;

 public:
  SnakeRender(ISnakeFrontendData* m, int rw = 10, int rh = 20) : model(m), renderW(rw), renderH(rh) {}
  void UpdateState(ISnakeFrontendData* m) { model = m; }
  void Draw() {
    int rowsPerSeg = renderH / model->Height();
    int colsPerSeg = renderW / model->Width();
    int acc = 0;
    for (const auto& pos : model->GetBody()) {
      acc += pos.y * rowsPerSeg + pos.x * colsPerSeg;
    }
    std::printf("%d\n", acc);
  }
};

class Controller {
  ISnakeModel* model;
  SnakeRender* render;

 public:
  Controller(ISnakeModel* m, SnakeRender* r) : model(m), render(r) {}
  void RunOnce() {
    ISnakeFrontendData* data = model->updateCurrentState(0);
    render->UpdateState(data);
    render->Draw();
  }
};

int main() {
  SnakeGame<10, 20> game;
  SnakeRender render(&game);
  Controller c(&game, &render);
  c.RunOnce();
  return 0;
}
