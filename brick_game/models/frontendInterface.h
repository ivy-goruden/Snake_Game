#pragma once
#include <list>

#include "../globals.h"
namespace s21 {
class Frontend_Interface {
 protected:
  int score = 0;
  int highScore = 0;
  int level = 0;
  int width = 0;
  int height = 0;

 public:
  virtual bool IsWin() = 0;
  virtual bool IsLose() = 0;
  int Width() const { return this->width; }
  int Height() const { return this->height; }
  int GetScore() const { return this->score; }
  int GetHighScore() const { return this->highScore; }
  int GetLevel() const { return this->level; }
  void Save_HIScore() {
    FILE* file = fopen("score.txt", "w");
    if (file == NULL) {
      perror("Error opening file");
      return;
    }
    int score = std::max(this->score, this->highScore);
    fprintf(file, "%d", score);
    fclose(file);
  };

  void Get_HIScore() {
    FILE* file = fopen("score.txt", "r");
    if (file != NULL) {
      if (fscanf(file, "%d", &this->highScore) != 1) {
        this->highScore = 0;
      }
      fclose(file);
    } else {
      this->highScore = 0;
    }
  }
  virtual ~Frontend_Interface() = default;
};
}  // namespace s21
