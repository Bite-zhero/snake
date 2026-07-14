#pragma once

#include "drawable.hpp"
#include "fruits.hpp"
#include <array>
#include <iostream>
#include <ncurses.h>
#include <vector>

void helloWorld();

enum Direcction {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

struct SnakeTail : public Drawable {
public:
  SnakeTail(int startX, int startY) : Drawable(startX, startY) {};

  void draw(WINDOW *win) override { mvaddch(y, x, '#'); }

  void undraw(WINDOW *win) override { mvaddch(y, x, ' '); }
};

class Snake : public Drawable {
public:
  int drawed = 0;
  Direcction direction = Direcction::UP;
  std::vector<SnakeTail> tail;
  int game_over = 0;

  Snake(int startX, int startY) : Drawable(startX, startY) {
    tail.push_back(SnakeTail(startX, startY));
  };

  void draw(WINDOW *win) override {
    mvaddch(y, x, ACS_CKBOARD );
    for (SnakeTail &snakeTail : tail) {
      snakeTail.draw(win);
    }
  }

  void undraw(WINDOW *win) override {
    mvaddch(y, x, ' ');
    for (SnakeTail &snakeTail : tail) {
      snakeTail.undraw(win);
    }
  }

  void move(WINDOW *win) {

    std::array<int, 2> old_coords = {x, y};
    std::array<int, 2> tmp_old_coords;
    for (SnakeTail &tail : tail) {
      tmp_old_coords = {tail.x, tail.y};
      tail.x = old_coords[0];
      tail.y = old_coords[1];
      old_coords = tmp_old_coords;
    }

    switch (direction) {
      using enum Direcction;
      case UP:
        y -= 1;
        break;

      case RIGHT:
        x += 1;
        break;

      case DOWN:
        y += 1;
        break;

      case LEFT:
        x -= 1;
        break;
    }
  }

  void increase_tail(int quantity) {
    tail.push_back(SnakeTail(tail.back().y, tail.back().x ));
  };
};