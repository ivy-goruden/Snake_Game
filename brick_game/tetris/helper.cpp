#include <array>
#include <list>
#include <vector>

#include "frontend.hpp"
#include "tetris.hpp"

namespace s21 {

void Tetris_Game::prepare_figures() {
  std::list<figure_t> figures;
  figures.push_back({3, 0, {{1, 1, 1, 1}}, 0});            // I
  figures.push_back({4, 0, {{1, 1}, {1, 1}}, 0});          // O
  figures.push_back({3, 0, {{0, 1, 0}, {1, 1, 1}}, 0});    // T
  figures.push_back({3, 0, {{1, 0}, {1, 0}, {1, 1}}, 0});  // L
  figures.push_back({3, 0, {{0, 1}, {0, 1}, {1, 1}}, 0});  // J
  figures.push_back({3, 0, {{0, 1, 1}, {1, 1, 0}}, 0});    // S
  figures.push_back({3, 0, {{1, 1, 0}, {0, 1, 1}}, 0});    // Z

  this->figures = figures;
}

void Tetris_Game::UpdateScore(int rows) {
  if (rows == 1) {
    this->score += 100;
  } else if (rows == 2) {
    this->score += 300;
  } else if (rows == 3) {
    this->score += 700;
  } else if (rows == 4) {
    this->score += 1500;
  }
  this->level = (int)(this->score / 600);
  this->timer = (1000000 - 1000000 * 0.1 * this->level);
}

void Tetris_Game::New_Figure(figure_t& figure) {
  int figure_num = rand() % 7;
  auto it = std::next(this->figures.begin(), figure_num);
  figure = *it;
}
void Tetris_Game::AccelerateFigure() {
  while (!collision(this->figure.x, this->figure.y + 1, this->figure.figure)) {
    this->figure.y++;
  }
}

void Tetris_Game::RotateFigure() {
  matrix_t& f = this->figure.figure;
  matrix_t newMatrix = matrix_t(f[0].size(), std::vector<int>(f.size()));
  for (int i = 0; i < int(f[0].size()); i++) {
    for (int x = 0; x < int(f.size()); x++) {
      newMatrix[i][x] = f[f.size() - 1 - x][i];
    }
  }
  if (!collision(this->figure.x, this->figure.y, newMatrix)) {
    this->figure.figure = newMatrix;
  }
}

void Tetris_Game::ValidateFixed() {
  auto filled = std::array<int, WIDTH>();
  filled.fill(1);
  int clearedRows = 0;
  for (int i = 0; i < HEIGHT; i++) {
    if (this->fixed[i] == filled) {
      this->fixed[i].fill(0);
      clearedRows++;
    }
  }
  UpdateScore(clearedRows);
  this->StackFixed();
}

void Tetris_Game::StackFixed(){
  std::array<std::array<int, WIDTH>, HEIGHT> newFixed;
  for (int i = HEIGHT - 1, j = HEIGHT - 1; i >= 0; i--) {
    if (this->fixed[i] != std::array<int, WIDTH>()) {
      newFixed[j--] = this->fixed[i];
    }
  }
  this->fixed = newFixed;
}
}  // namespace s21
