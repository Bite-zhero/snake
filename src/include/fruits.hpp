#pragma once

#include "drawable.hpp"
#include <ncurses.h>

class Fruit : public Drawable {
    public:
        Fruit(int startX, int startY) : Drawable(startX, startY) {};

        void draw(WINDOW* win) override {
            mvaddch(y, x, 'o' | A_BOLD);
        }

        void undraw(WINDOW* win) override {
            mvaddch(y, x, ' ');
        }
};