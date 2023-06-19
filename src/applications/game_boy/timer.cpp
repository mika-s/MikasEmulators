#include "timer.h"
#include "applications/game_boy/interfaces/interrupt_observer.h"
#include "applications/game_boy/interrupts.h"
#include "crosscutting/util/byte_util.h"
#include <algorithm>
#include <fmt/core.h>
#include <stdexcept>

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void Timer::update(cyc cycles)
{
    m_internal_divider_counter += cycles;
    if (m_internal_divider_counter >= 0xff) {
        m_internal_divider_counter = 0;
        ++m_divider;
    }

    if (m_is_running) {
        m_internal_counter -= cycles;

        if (m_internal_counter <= 0) {
            switch (m_timer_clock_speed) {
            case TimerClockSpeed::_4096Hz:
                m_internal_counter = 1024;
                break;
            case TimerClockSpeed::_262144Hz:
                m_internal_counter = 16;
                break;
            case TimerClockSpeed::_65536Hz:
                m_internal_counter = 64;
                break;
            case TimerClockSpeed::_16384Hz:
                m_internal_counter = 256;
                break;
            }

            if (m_counter == 0xff) {
                m_counter = m_modulo;
                notify_interrupt_observers(TIMER);
            } else {
                ++m_counter;
            }
        }
    }
}

u8 Timer::divider() const
{
    return m_divider;
}

void Timer::reset_divider()
{
    m_divider = 0;
}

u8 Timer::counter() const
{
    return m_counter;
}

void Timer::counter(u8 new_value)
{
    m_counter = new_value;
}

u8 Timer::modulo() const
{
    return m_modulo;
}

void Timer::modulo(u8 new_value)
{
    m_modulo = new_value;
}

u8 Timer::control() const
{
    u8 value = 0;

    if (m_is_running) {
        set_bit(value, s_timer_enabled_bit);
    }

    switch (m_timer_clock_speed) {
    case TimerClockSpeed::_4096Hz:
        break;
    case TimerClockSpeed::_262144Hz:
        set_bit(value, 0);
        break;
    case TimerClockSpeed::_65536Hz:
        set_bit(value, 1);
        break;
    case TimerClockSpeed::_16384Hz:
        set_bit(value, 0);
        set_bit(value, 1);
        break;
    }

    return value;
}

void Timer::control(u8 new_value)
{
    m_is_running = is_bit_set(new_value, s_timer_enabled_bit);

    u8 const speed = 0b00000011 & new_value;
    switch (speed) {
    case 0b00:
        m_timer_clock_speed = TimerClockSpeed::_4096Hz;
        break;
    case 0b01:
        m_timer_clock_speed = TimerClockSpeed::_262144Hz;
        break;
    case 0b10:
        m_timer_clock_speed = TimerClockSpeed::_65536Hz;
        break;
    case 0b11:
        m_timer_clock_speed = TimerClockSpeed::_16384Hz;
        break;
    default:
        throw std::runtime_error(
            fmt::format(
                "Programming error: Timer speed of {} should never be possible. Legal values are 00, 01, 10 and 11.",
                speed));
    }
}
void Timer::add_interrupt_observer(InterruptObserver& observer)
{
    m_interrupt_observers.push_back(&observer);
}

void Timer::remove_interrupt_observer(InterruptObserver* observer)
{
    m_interrupt_observers.erase(
        std::remove(m_interrupt_observers.begin(), m_interrupt_observers.end(), observer),
        m_interrupt_observers.end());
}

void Timer::notify_interrupt_observers(Interrupts interrupt)
{
    for (InterruptObserver* observer : m_interrupt_observers) {
        observer->interrupt(interrupt);
    }
}

}
