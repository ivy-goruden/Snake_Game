#ifndef SNAKE_DESKTOP_RENDER_H
#define SNAKE_DESKTOP_RENDER_H

#include <QKeyEvent>
#include <QMainWindow>
#include <QPainter>
#include <QStackedWidget>
#include <QTextEdit>

#include "../../../brick_game/globals.h"
#include "../../../brick_game/models/render.h"
#include "../../../brick_game/snake/snake.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class Snake_Desktop_Render;
}
QT_END_NAMESPACE
namespace s21 {
class Snake_Desktop_Render : public QMainWindow, public s21::Render {
  Q_OBJECT
  enum State { ST_WAITING, ST_MOVE, ST_PAUSE, ST_WIN, ST_LOSE };

 public:
  explicit Snake_Desktop_Render(s21::SnakeFrontendData* mdl,
                                QWidget* parent = nullptr);
  ~Snake_Desktop_Render() override;

  UserAction_t GetAction() override;
  void SendAction(UserAction_t action);
  void UpdateState(s21::Frontend_Interface* model) override;
  State Get_State();
  bool isRunning();

 private slots:
  void on_tabWidget_currentChanged(int index);
 signals:
  void keyPressed(UserAction_t action);

 private:
  s21::SnakeFrontendData* model;
  Ui::Snake_Desktop_Render* ui;
  int W_HEIGHT = 600;
  int W_WIDTH = 800;
  void keyPressEvent(QKeyEvent* event) override;
  void paintEvent(QPaintEvent* event) override;
  typedef bool (Snake_Desktop_Render::*InputHandler)(int);
  typedef void (Snake_Desktop_Render::*ScreenHandler)(QPainter*);
  int height;
  int width;
  int start_y;
  int start_x;
  State cur_state;
  std::map<State, InputHandler> input_handlers = {
      {ST_WAITING, &Snake_Desktop_Render::Waiting_Handler},
      {ST_MOVE, &Snake_Desktop_Render::Move_Handler},
      {ST_PAUSE, &Snake_Desktop_Render::Pause_Handler},
      {ST_WIN, &Snake_Desktop_Render::Win_Handler},
      {ST_LOSE, &Snake_Desktop_Render::Lose_Handler}};
  std::map<State, ScreenHandler> screen_handlers = {
      {ST_WAITING, &Snake_Desktop_Render::WaitingScreen_Handler},
      {ST_MOVE, &Snake_Desktop_Render::MoveScreen_Handler},
      {ST_PAUSE, &Snake_Desktop_Render::PauseScreen_Handler},
      {ST_WIN, &Snake_Desktop_Render::WinScreen_Handler},
      {ST_LOSE, &Snake_Desktop_Render::LoseScreen_Handler}};
  bool Waiting_Handler(int);
  bool Move_Handler(int);
  bool Pause_Handler(int);
  bool Win_Handler(int);
  bool Lose_Handler(int);

  void WaitingScreen_Handler(QPainter* paint);
  void MoveScreen_Handler(QPainter* paint);
  void PauseScreen_Handler(QPainter* paint);
  void WinScreen_Handler(QPainter* paint);
  void LoseScreen_Handler(QPainter* paint);
  void Draw_Stats();
  InputHandler getInputHandler();
  void Draw();
};
}  // namespace s21
#endif  // SNAKE_DESKTOP_RENDER_H
