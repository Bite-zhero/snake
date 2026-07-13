#pragma once

#include "drawable.hpp"
#include "fruits.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>
#include <vector>

void helloWorld();

struct SnakeTail : public Drawable {
public:
  SnakeTail(int startX, int startY) : Drawable(startX, startY) {};

  void draw(WINDOW *win) override { mvaddch(y, x, ' '); }

  void undraw(WINDOW *win) override { mvaddch(y, x, ' '); }
};

class Snake : public Drawable {
public:
  int drawed = 0;
  int direction = 1;
  std::vector<SnakeTail> tail;
  int game_over = 0;

  Snake(int startX, int startY) : Drawable(startX, startY) {
    tail.push_back(SnakeTail(startX, startY));
  };

  void draw(WINDOW *win) override {
    wattron(win, COLOR_PAIR(SNAKE_COLOR));
    mvaddch(y, x, ' ');
    for (SnakeTail &snakeTail : tail) {
      snakeTail.draw(win);
    }
    wattroff(win, COLOR_PAIR(SNAKE_COLOR));
  }

  void undraw(WINDOW *win) override {
    mvaddch(y, x, ' ');
    for (SnakeTail &snakeTail : tail) {
      snakeTail.undraw(win);
    }
  }

  void move(WINDOW *win) {

    undraw(win);

    std::array<int, 2> old_coords = {x, y};
    std::array<int, 2> tmp_old_coords;
    for (SnakeTail &tail : tail) {
      tmp_old_coords = {tail.x, tail.y};
      tail.x = old_coords[0];
      tail.y = old_coords[1];
      old_coords = tmp_old_coords;
    }

    switch (direction) {
    case 1:
      y -= 1;
      break;

    case 2:
      x -= 1;
      break;

    case 3:
      y += 1;
      break;

    case 4:
      x += 1;
      break;
    }

    draw(win);

    for (SnakeTail &tail : tail) {
      if (x == tail.x && y == tail.y) {
        game_over_function();
      }
    }
  }

  void increase_tail(int quantity) {
    tail.push_back(SnakeTail(tail.back().y - 1, tail.back().x - 1));
  };

  void game_over_function() { game_over = 1; };
};