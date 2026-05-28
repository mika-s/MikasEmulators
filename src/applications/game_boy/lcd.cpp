#include "lcd.h"
#include "applications/game_boy/interfaces/interrupt_observer.h"
#include "applications/game_boy/interrupts.h"
#include "lcd_status.h"
#include <algorithm>

namespace emu::applications::game_boy {
class LcdControl;
}

namespace emu::applications::game_boy {

auto Lcd::lcd_control() -> LcdControl&
{
    return m_lcd_control;
}

auto Lcd::lcd_status() -> LcdStatus&
{
    return m_lcd_status;
}

void Lcd::increment_scanline()
{
    ++m_ly;

    m_lcd_status.m_is_lyc_eq_ly = m_ly == m_lyc;
    if (m_lcd_status.m_is_lyc_eq_ly) {
        notify_interrupt_observers(LCD);
    }
}

void Lcd::reset_scanline()
{
    m_ly = 0;
}

void Lcd::add_interrupt_observer(InterruptObserver& observer)
{
    m_interrupt_observers.push_back(&observer);
}

void Lcd::remove_interrupt_observer(InterruptObserver* observer)
{
    m_interrupt_observers.erase(
        std::remove(m_interrupt_observers.begin(), m_interrupt_observers.end(), observer),
        m_interrupt_observers.end());
}

void Lcd::notify_interrupt_observers(const Interrupts interrupt) const
{
    for (InterruptObserver* observer : m_interrupt_observers) {
        observer->interrupt(interrupt);
    }
}

}
