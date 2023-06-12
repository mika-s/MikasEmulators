#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::game_boy {

enum class LcdStatusMode {
    HBLANK,
    VBLANK,
    SEARCHING_SPRITES_ATTR,
    TRANSFERRING_DATA_TO_LCD_DRIVER,
    UNKNOWN,
};

class LcdStatus {
public:
    bool m_is_lyc_eq_ly_stat_interrupt_source { false };
    bool m_is_mode_2_oam_stat_interrupt_source { false };
    bool m_is_mode_1_vblank_stat_interrupt_source { false };
    bool m_is_mode_0_hblank_stat_interrupt_source { false };
    bool m_is_lyc_eq_ly { false };
    LcdStatusMode m_lcd_status_mode;

    void update_from_memory(u8 value);

    u8 to_u8();

private:
    static unsigned int const s_lyc_eq_ly_stat_interrupt_source_bit = 6;
    static unsigned int const s_mode_2_oam_stat_interrupt_source_bit = 5;
    static unsigned int const s_mode_1_vblank_stat_interrupt_source_bit = 4;
    static unsigned int const s_mode_0_hblank_stat_interrupt_source_bit = 3;
    static unsigned int const s_lyc_eq_ly_bit = 2;
    static unsigned int const s_lcd_status_mode_bit_pattern = 0b00000011;
};
}
