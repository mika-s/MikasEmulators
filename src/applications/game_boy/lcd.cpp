#include "lcd.h"
#include "lcd_status.h"

namespace emu::applications::game_boy {
class LcdControl;
}

namespace emu::applications::game_boy {

LcdControl& Lcd::lcd_control()
{
    return m_lcd_control;
}

LcdStatus& Lcd::lcd_status()
{
    return m_lcd_status;
}

void Lcd::increment_scanline()
{
    ++m_ly;

    m_lcd_status.m_is_lyc_eq_ly = m_ly == m_lyc; // TODO: interrupt if true
}

void Lcd::reset_scanline()
{
    m_ly = 0;
}

}
