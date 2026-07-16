#include "config.hpp"
#include "fruits.hpp"
#include "game.hpp"
#include "snake.hpp"
#include <chrono>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <random>
#include <string>
#include <thread>

void walls_generate() {

    clear();

    int maxY = LINES;
    int maxX = COLS;

    for (int i = 0; i <= maxX; i++) {
        mvaddch(0, i, ACS_BLOCK);
        mvaddch(maxY - 1, i, ACS_BLOCK);
    }
    for (int i = 0; i < maxY; i++) {
        mvaddch(i, 0, ACS_BLOCK);
        mvaddch(i, maxX - 1, ACS_BLOCK);
    }
    refresh();
}

int Game::next_key_from_buffer() {
    if (!keys_buffer.front()) {
        return 1;
    }
    int key = keys_buffer.front();
    keys_buffer.erase(keys_buffer.begin() + 1);
    return key;
}

void Game::sleep() {
    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000 / config.ticks_per_second));
}

void Game::input_handler() {
    int key = next_key_from_buffer();
    switch (key) {
    case 'q':
        game_running_status = 0;
        set_max_score();
        break;

    case 'Q':
        game_running_status = 0;
        set_max_score();
        break;

    case KEY_RESIZE:
        clear();
        width = getmaxx(win);
        height = getmaxy(win);
        walls_generate();
        refresh();
        break;

    case 'r':
        for (Fruit &fruit : fruits) {
            fruit.undraw(win);
        };
        fruits_generator(config.fruits_to_generate);
        break;

    case KEY_UP:
        if (snake.direction != Direcction::DOWN) {
            snake.direction = Direcction::UP;
        };
        break;

    case KEY_RIGHT:
        if (snake.direction != Direcction::LEFT) {
            snake.direction = Direcction::RIGHT;
        };
        break;

    case KEY_DOWN:
        if (snake.direction != Direcction::UP) {
            snake.direction = Direcction::DOWN;
        };
        break;

    case KEY_LEFT:
        if (snake.direction != Direcction::RIGHT) {
            snake.direction = Direcction::LEFT;
        };
        break;

    case 'w':
        if (snake.direction != Direcction::DOWN) {
            snake.direction = Direcction::UP;
        };
        break;

    case 'd':
        if (snake.direction != Direcction::LEFT) {
            snake.direction = Direcction::RIGHT;
        };
        break;

    case 's':
        if (snake.direction != Direcction::UP) {
            snake.direction = Direcction::DOWN;
        };
        break;

    case 'a':
        if (snake.direction != Direcction::RIGHT) {
            snake.direction = Direcction::LEFT;
        };
        break;
    }
}

void Game::initScr() {
    initscr();
    win = stdscr;
    noecho();
    cbreak();
    keypad(win, TRUE);
    nodelay(win, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(WALLS_COLOR, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(SNAKE_COLOR, COLOR_GREEN, COLOR_GREEN);
    }
    walls_generate();
    wrefresh(win);
    sleep();
}

void Game::fruits_generator(int fruits_quatity) {
    int heigth = getmaxy(win);
    int width = getmaxx(win);
    int totalCoords = width * heigth;
    std::vector<Fruit> _fruits;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, totalCoords);
    for (int i = 0; i < fruits_quatity; ++i) {
        int rawCoords = distr(gen);
        // std::cout << "a fruit gen iteration" << std::endl;

        int tmpY = rawCoords / width;
        int tmpX = rawCoords % width;
        if ((tmpX == 0 || tmpX >= width - 1) ||
            (tmpY == 0 || tmpY >= heigth - 1)) {
            --i;
            continue;
        }
        for (Fruit &fruit : _fruits) {
            if (tmpX == fruit.x && tmpY == fruit.y) {
                --i;
                continue;
            }
        }
        _fruits.push_back(Fruit(tmpX, tmpY));
    }
    fruits = _fruits;
}

void Game::snake_move() {
    if (passed_ticks == config.snake_update_time_factor) {
        passed_ticks = 0;
        snake.undraw(win);
        snake.move(win);
    }
}

void Game::draw() {
    for (Fruit &fruit : fruits) {
        fruit.draw(win);
    }
    snake.draw(win);
    std::string scoreStr = std::format("SCORE: {}", score);
    std::string max_scoreStr = std::format("MAX SCORE: {}", max_score);
    mvaddstr(0, (width / 2) - scoreStr.size() / 2, scoreStr.c_str());
    mvaddstr(height - 1, (width / 2) - scoreStr.size() / 2,
             max_scoreStr.c_str());
}

void Game::collisions_check() {
    for (int i = 0; i < fruits.size(); i++) {
        if (fruits[i].y == snake.y && fruits[i].x == snake.x) {
            fruits.erase(fruits.begin() + i);
            snake.increase_tail(1);
            score++;
        }
    }
    if (snake.y <= 0 || snake.y >= height) {
        game_over();
    }
    if (snake.x <= 0 || snake.x >= width) {
        game_over();
    }
    for (SnakeTail &snakeTail : snake.tail) {
        if (snake.x == snakeTail.x && snake.y == snakeTail.y) {
            game_over();
        }
    }
}

void Game::set_max_score() {
    std::ofstream dataFile("dataFile.txt");
    if (!dataFile.is_open()) {
        return;
    }
    dataFile << "Max score: " << max_score << std::endl;
    dataFile.close();
}

void Game::get_max_score() {
    std::ifstream dataFile("dataFile.txt");
    if (!dataFile.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(dataFile, line)) {
        std::string rstr = line.substr(11, line.size() - 1);
        max_score = std::stoi(rstr);
    }
    dataFile.close();
}

void Game::game_over() {
    endwin();
    set_max_score();
    std::cout << "Game Over :(" << std::endl;
    std::exit(0);
}

void Game::run() {
    initScr();
    width = getmaxx(win);
    height = getmaxy(win);
    snake.x = width / 2;
    snake.y = height / 2;
    get_max_score();
    while (game_running_status == 1) {
        passed_ticks++;

        keys_buffer.push_back(getch());
        input_handler();

        if (fruits.size() <= 0) {
            fruits_generator(16);
        }

        snake_move();

        collisions_check();

        if (score > max_score) {
            max_score = score;
        }

        draw();
        refresh();
        sleep();
    }
}

int main(int argc, char *argv[]) {

    GameConfig configs;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--ticks") {
            configs.ticks_per_second = std::stoi(argv[i + 1]);
        }
        if (arg == "--fruits") {
            configs.fruits_to_generate = std::stoi(argv[i + 1]);
        }
        if (arg == "--snake-update-factor") {
            configs.snake_update_time_factor = std::stoi(argv[i + 1]);
        }
    }

    Game game(configs);

    game.run();

    endwin();
    return 0;
}
