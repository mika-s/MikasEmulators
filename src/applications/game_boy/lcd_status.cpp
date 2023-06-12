#include "lcd_status.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void LcdStatus::update_from_memory(u8 value)
{
    switch (value & s_lcd_status_mode_bit_pattern) {
    case 0:
        m_lcd_status_mode = LcdStatusMode::HBLANK;
        break;
    case 1:
        m_lcd_status_mode = LcdStatusMode::VBLANK;
        break;
    case 2:
        m_lcd_status_mode = LcdStatusMode::SEARCHING_SPRITES_ATTR;
        break;
    case 3:
        m_lcd_status_mode = LcdStatusMode::TRANSFERRING_DATA_TO_LCD_DRIVER;
        break;
    default:
        m_lcd_status_mode = LcdStatusMode::UNKNOWN;
        break;
    }

    m_is_lyc_eq_ly = is_bit_set(value, s_lyc_eq_ly_bit);
    m_is_mode_0_hblank_stat_interrupt_source = is_bit_set(value, s_mode_0_hblank_stat_interrupt_source_bit);
    m_is_mode_1_vblank_stat_interrupt_source = is_bit_set(value, s_mode_1_vblank_stat_interrupt_source_bit);
    m_is_mode_2_oam_stat_interrupt_source = is_bit_set(value, s_mode_2_oam_stat_interrupt_source_bit);
    m_is_lyc_eq_ly_stat_interrupt_source = is_bit_set(value, s_lyc_eq_ly_stat_interrupt_source_bit);
}

u8 LcdStatus::to_u8()
{
    u8 value = 0;

    switch (m_lcd_status_mode) {
    case LcdStatusMode::HBLANK:
        value = 0;
        break;
    case LcdStatusMode::VBLANK:
        value = 1;
        break;
    case LcdStatusMode::SEARCHING_SPRITES_ATTR:
        value = 2;
        break;
    case LcdStatusMode::TRANSFERRING_DATA_TO_LCD_DRIVER:
        value = 3;
        break;
    case LcdStatusMode::UNKNOWN:
        break;
    }

    if (m_is_lyc_eq_ly) {
        set_bit(value, s_lyc_eq_ly_bit);
    }
    if (m_is_mode_0_hblank_stat_interrupt_source) {
        set_bit(value, s_mode_0_hblank_stat_interrupt_source_bit);
    }
    if (m_is_mode_1_vblank_stat_interrupt_source) {
        set_bit(value, s_mode_1_vblank_stat_interrupt_source_bit);
    }
    if (m_is_mode_2_oam_stat_interrupt_source) {
        set_bit(value, s_mode_2_oam_stat_interrupt_source_bit);
    }
    if (m_is_lyc_eq_ly_stat_interrupt_source) {
        set_bit(value, s_lyc_eq_ly_stat_interrupt_source_bit);
    }

    return value;
}

}
