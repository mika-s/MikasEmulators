#include "lcd.h"

namespace emu::applications::game_boy {
class LcdControl;
class LcdStatus;
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

}
