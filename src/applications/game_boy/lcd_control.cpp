#include "lcd_control.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void LcdControl::update_from_memory(u8 value)
{
    m_is_ldc_and_ppu_enabled = is_bit_set(value, s_ldc_and_ppu_enabled_bit);
    m_window_tile_map_area = is_bit_set(value, s_window_tile_map_area_bit) ? 1 : 0;
    m_is_window_enabled = is_bit_set(value, s_window_enable_bit);
    m_bg_and_window_tile_data_area = is_bit_set(value, s_bg_and_window_tile_data_area_bit) ? 1 : 0;
    m_bg_tile_map_area = is_bit_set(value, s_bg_tile_map_area_bit) ? 1 : 0;
    m_obj_size = is_bit_set(value, s_obj_size_bit) ? 1 : 0;
    m_is_obj_enabled = is_bit_set(value, s_obj_enable_bit);
    m_is_bg_and_window_enabled = is_bit_set(value, s_bg_and_window_enable_bit);
}

u8 LcdControl::to_u8()
{
    u8 value = 0;

    if (m_is_ldc_and_ppu_enabled) {
        set_bit(value, s_ldc_and_ppu_enabled_bit);
    }
    if (m_window_tile_map_area) {
        set_bit(value, s_window_tile_map_area_bit);
    }
    if (m_is_window_enabled) {
        set_bit(value, s_window_enable_bit);
    }
    if (m_bg_and_window_tile_data_area) {
        set_bit(value, s_bg_and_window_tile_data_area_bit);
    }
    if (m_bg_tile_map_area) {
        set_bit(value, s_bg_tile_map_area_bit);
    }
    if (m_obj_size) {
        set_bit(value, s_obj_size_bit);
    }
    if (m_is_obj_enabled) {
        set_bit(value, s_obj_enable_bit);
    }
    if (m_is_bg_and_window_enabled) {
        set_bit(value, s_bg_and_window_enable_bit);
    }

    return value;
}

}
