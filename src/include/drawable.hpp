#pragma once

#include <initializer_list>
#include <ncurses.h>
class Drawable {
public:
  int x, y;
  static const int SNAKE_COLOR = 1;
  static const int FRUIT_COLOR = 2;

  Drawable(int _x, int _y) : x(_x), y(_y) {
    if (has_colors()) {
      start_color();
      use_default_colors();

      init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_GREEN);
      init_pair(FRUIT_COLOR, COLOR_RED, COLOR_RED);
    }
  };
  virtual ~Drawable() = default;

  virtual void draw(WINDOW *win) = 0;

  virtual void undraw(WINDOW *win) = 0;
};