#include <list>
#include <vector>

#include "frontend.hpp"
#include "tetris.hpp"

namespace s21 {

void Tetris_Game::prepare_figures() {
  std::list<figure_t> figures;

  // I
  figures.push_back({3, 0, {{1, 1, 1, 1}}, 0});

  // O
  figures.push_back({4, 0, {{1, 1}, {1, 1}}, 0});

  // T
  figures.push_back({3, 0, {{0, 1, 0}, {1, 1, 1}}, 0});

  // L
  figures.push_back({3, 0, {{1, 0}, {1, 0}, {1, 1}}, 0});

  // J
  figures.push_back({3, 0, {{0, 1}, {0, 1}, {1, 1}}, 0});

  // S
  figures.push_back({3, 0, {{0, 1, 1}, {1, 1, 0}}, 0});

  // Z
  figures.push_back({3, 0, {{1, 1, 0}, {0, 1, 1}}, 0});

  this->figures = figures;
}

}  // namespace s21
