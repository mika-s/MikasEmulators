#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::game_boy {

class LcdControl {
public:
    bool is_ldc_and_ppu_enabled { false };
    int window_tile_map_area { 0 };
    bool is_window_enabled { false };
    int bg_and_window_tile_data_area { 0 };
    int bg_tile_map_area { 0 };
    int obj_size { false };
    bool is_obj_enabled { false };
    bool is_bg_and_window_enabled { false };

    void update_from_memory(u8 value);

    u8 to_u8();

private:
    static const unsigned int s_ldc_and_ppu_enabled_bit = 7;
    static const unsigned int s_window_tile_map_area_bit = 6;
    static const unsigned int s_window_enable_bit = 5;
    static const unsigned int s_bg_and_window_tile_data_area_bit = 4;
    static const unsigned int s_bg_tile_map_area_bit = 3;
    static const unsigned int s_obj_size_bit = 2;
    static const unsigned int s_obj_enable_bit = 1;
    static const unsigned int s_bg_and_window_enable_bit = 0;
};

}
