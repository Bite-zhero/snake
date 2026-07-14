#pragma once

struct GameConfig {
    int ticks_per_second = 12;
    int fruits_to_generate = 16;
    int snake_update_time_factor = ticks_per_second / 6;
};