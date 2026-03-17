#include "globals.h"

#include <stdarg.h>
#include <stdio.h>

void init_log() {
  FILE* file = fopen("log.txt", "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  fclose(file);
}

void write_log(const char* fmt, ...) {
  FILE* file = fopen("log.txt", "a");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }
  va_list args;
  va_start(args, fmt);
  vfprintf(file, fmt, args);
  fputc('\n', file);
  va_end(args);
  fclose(file);
}

bool Position::operator==(const Position& other) const {
  return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
  return x != other.x || y != other.y;
}

bool Position::operator<(const Position& other) const {
  if (x != other.x) {
    return x < other.x;
  }
  return y < other.y;
}
