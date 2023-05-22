#include "timer.h"
#include "crosscutting/util/byte_util.h"
#include <fmt/core.h>
#include <stdexcept>

namespace emu::applications::game_boy {

using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void Timer::inc([[maybe_unused]] cyc cycles)
{
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

}
