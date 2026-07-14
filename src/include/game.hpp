#pragma once

#include "config.hpp"
#include "fruits.hpp"
#include "snake.hpp"
#include <iostream>
#include <ncurses.h>
#include <vector>

class Game {
    public:
        GameConfig config;

        Game(GameConfig _config) {config = _config;};
        void run();

        void sleep();

        void first_fruits_gen() {fruits_generator(config.fruits_to_generate);};

    private:
        WINDOW* win;
        int width;
        int height;
        int game_running_status = 1;
        std::vector<int> keys_buffer;
        int WALLS_COLOR = 1;
        int SNAKE_COLOR = 2;
        int FRUITS_COLOR = 3;
        std::vector<Fruit> fruits;
        Snake snake = Snake(0, 0);
        int passed_ticks = 0;
        int score = 0;
        int max_score = 0; 

        int next_key_from_buffer();

        void initScr();

        void input_handler();

        void fruits_generator(int fruits_quantity);

        void snake_move();

        void draw();
        
        void collisions_check();

        void game_over();

        void speed_factor_calculation();

        void set_max_score();

        void get_max_score();

};