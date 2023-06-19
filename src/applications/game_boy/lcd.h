#pragma once

#include "applications/game_boy/interrupts.h"
#include "crosscutting/typedefs.h"
#include "lcd_control.h"
#include "lcd_status.h"
#include <vector>

namespace emu::applications::game_boy {
class InterruptObserver;
}

namespace emu::applications::game_boy {

class Lcd {
public:
    [[nodiscard]] LcdControl& lcd_control();

    [[nodiscard]] LcdStatus& lcd_status();

    void increment_scanline();

    void reset_scanline();

    void add_interrupt_observer(InterruptObserver& observer);

    void remove_interrupt_observer(InterruptObserver* observer);

    void notify_interrupt_observers(Interrupts interrupt);

    u8 m_ly { 0 };
    u8 m_lyc { 0 };
    u8 m_scy { 0 };
    u8 m_scx { 0 };
    u8 m_wy { 0 };
    u8 m_wx { 0 };

private:
    LcdControl m_lcd_control;
    LcdStatus m_lcd_status;

    std::vector<InterruptObserver*> m_interrupt_observers;
};
}
