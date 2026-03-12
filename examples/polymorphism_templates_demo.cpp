#include <cstdio>
#include <list>

struct Position {
  int x;
  int y;
  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }
};

class IModel {
 public:
  virtual int Width() const = 0;
  virtual int Height() const = 0;
  virtual std::list<Position> GetBody() const = 0;
  virtual ~IModel() = default;
};

template <int W, int H>
class SnakeGame : public IModel {
 public:
  int Width() const override { return W; }
  int Height() const override { return H; }
  std::list<Position> GetBody() const override {
    return {{W / 2, H / 2}, {W / 2 - 1, H / 2}};
  }
};

class SnakeRender {
  IModel* model;
  int renderW;
  int renderH;

 public:
  SnakeRender(IModel* m, int rw, int rh) : model(m), renderW(rw), renderH(rh) {}
  void UpdateState(IModel* m) { model = m; }
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
  IModel* model;
  SnakeRender* render;

 public:
  Controller(IModel* m, SnakeRender* r) : model(m), render(r) {}
  void RunOnce() {
    render->UpdateState(model);
    render->Draw();
  }
};

int main() {
  SnakeGame<10, 20> game;
  SnakeRender render(&game, 20, 10);
  Controller c(&game, &render);
  c.RunOnce();
  return 0;
}
