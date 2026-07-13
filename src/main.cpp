#include "fruits.hpp"
#include "include/snake.hpp"
#include "logic.hpp"
#include "snake.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <iterator>
#include <ncurses.h>
#include <random>
#include <string>
#include <thread>
#include <vector>
using namespace std;

void helloWorld() { std::cout << "hello World :)" << std::endl; }

// Damn, i code this function before acknowled that is a built-in function what
// does the same, fuck
void walls_generate() {

    // refresh();
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

vector<Fruit> fruits_generator(int tC, int fruit_quantity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1, tC - 1);
    std::vector<array<int, 2>> fruitsCoords;
    std::vector<Fruit> fruits;

    int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);

    for (int i = 0; i < fruit_quantity; i++) {
        int tmpTotalCoord = distr(gen);

        int tmpX = tmpTotalCoord % width;
        int tmpY = tmpTotalCoord / width;

        array<int, 2> tmpArr;

        tmpArr[0] = tmpX;
        tmpArr[1] = tmpY;

        if (tmpArr[0] == 0) {
            tmpArr[0]++;
        }
        if (tmpArr[0] >= width) {
            tmpArr[0]--;
        }
        if (tmpArr[1] == 0) {
            tmpArr[1]++;
        }
        if (tmpArr[1] >= height) {
            tmpArr[1]--;
        }

        fruitsCoords.push_back(tmpArr);
    }
    for (array<int, 2> fruitCoord : fruitsCoords) {
        fruits.push_back(Fruit(fruitCoord[0], fruitCoord[1]));
    }
    return fruits;
}

void fruit_draw(vector<Fruit> &input_fruits) {
    for (Fruit &fruit : input_fruits) {
        fruit.draw(stdscr);
    }
}

void tick(int &program_running_status, int tick_per_second, vector<Fruit> &fruits, int &f_first_gen, Snake &snake, int &ticks_meter, int &fruits_quantity_gen, int snake_update_time_factor) {

    if (snake.drawed != 1) {
        snake.draw(stdscr);
        snake.drawed = 1;
    }

    int k = getch();

    auto fruit_undraw = [](vector<Fruit> &input_fruits) -> void {
        for (Fruit &fruit : input_fruits) {
            fruit.undraw(stdscr);
        }
    };
    fruit_draw(fruits);

    ticks_meter++;

    ///// START THE TICK PROCCESSOR
    if (k != ERR) {
        if (k == 'q' || k == 'Q') {
            program_running_status = 0;
        } else if (k == KEY_RESIZE) {
            walls_generate();
            fruit_undraw(fruits);
            fruits = fruits_generator((getmaxx(stdscr) - 1) * (getmaxy(stdscr) - 1),fruits_quantity_gen);
            snake.drawed = 0;
        } else if (k == 'R' || k == 'r') {
            fruit_undraw(fruits);
            fruits = fruits_generator((getmaxx(stdscr) - 1) * (getmaxy(stdscr) - 1),fruits_quantity_gen);
        } else if (k == KEY_UP && snake.direction != 3) {
            snake.direction = 1;
        } else if (k == KEY_LEFT && snake.direction != 4) {
            snake.direction = 2;
        } else if (k == KEY_DOWN && snake.direction != 1) {
            snake.direction = 3;
        } else if (k == KEY_RIGHT && snake.direction != 2) {
            snake.direction = 4;
        }
    }

    int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);

    if (ticks_meter >= tick_per_second / snake_update_time_factor) {
        snake.move(stdscr);
        ticks_meter = 0;
    }

    for (int i = 0; i < fruits.size(); i++) {
        if (fruits[i].y == snake.y && fruits[i].x == snake.x) {
            // fruits[i].undraw(stdscr);
            fruits.erase(fruits.begin() + i);
            snake.increase_tail(1);
        }
    }

    // cout << "width: " << width << "  " << "height: " << height << endl;
    if ((snake.y == 0 || snake.y == height - 1) ||
        (snake.x == 0 || snake.x == width - 1)) {
        snake.game_over_function();
    }

    if (snake.game_over != 0) {
        program_running_status = 0;
    }

    if (fruits.size() <= 0) {
        fruits = fruits_generator((getmaxx(stdscr) - 1) * (getmaxy(stdscr) - 1), fruits_quantity_gen);
    }

    std::this_thread::sleep_for(chrono::milliseconds(1000 / tick_per_second));
};

int main(int argc, char *argv[]) {

    int tickPerSecond = 12;
    int fruits_to_generate = 16;
    int snake_update_time_factor = tickPerSecond / 2;
    for (int i = 0; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--ticks") {
            tickPerSecond = std::stoi(argv[i+1]);           
        }
        if (arg == "--fruits") {
            fruits_to_generate = std::stoi(argv[i+1]);
        }
        if (arg == "--snake-update-factor") {
            snake_update_time_factor = std::stoi(argv[i+1]);
        }
    }

    //// init program stats
    int program_running_status = 1;

    initscr(); // that thing initiate the screen, i thought that this are for
               // everything on ncurses
    noecho();  // the name say what its do, dont echo the characters to the
               // terminal again
    cbreak();

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    curs_set(0);

    /////////////

    int width = getmaxx(stdscr);
    int height = getmaxy(stdscr);

    int total_avaible_space = (width - 1) * (height - 1);

    Snake snake(width / 2, height / 2);

    vector<Fruit> fruits = fruits_generator(total_avaible_space, fruits_to_generate);

    walls_generate();

    int totalCoords = width * height;


    int passedTicks = 0;

    /////////////

    int ola = 0;
    int chr;
    while (program_running_status == 1) {
        tick(program_running_status, tickPerSecond, fruits, ola, snake, passedTicks, fruits_to_generate, snake_update_time_factor);
    }

    endwin();

    if (snake.game_over == 1) {
        cout << "game over :(" << endl;
    }

    // cout << "an succes on execution :)\n" << program_running_status << endl;

    return 1;
}