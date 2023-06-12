#pragma once

#include "crosscutting/typedefs.h"
#include "lcd_control.h"
#include "lcd_status.h"

namespace emu::applications::game_boy {

class Lcd {
public:
    [[nodiscard]] LcdControl& lcd_control();

    [[nodiscard]] LcdStatus& lcd_status();

    void increment_scanline();

    void reset_scanline();

    u8 m_ly { 0 };
    u8 m_lyc { 0 };
    u8 m_scy { 0 };
    u8 m_scx { 0 };
    u8 m_wy { 0 };
    u8 m_wx { 0 };

private:
    LcdControl m_lcd_control;
    LcdStatus m_lcd_status;
};
}
