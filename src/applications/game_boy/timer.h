#pragma once

#include "crosscutting/typedefs.h"

namespace emu::applications::game_boy {

enum class TimerClockSpeed {
    _4096Hz,
    _262144Hz,
    _65536Hz,
    _16384Hz
};

class Timer {
public:
    void inc(cyc cycles);

    [[nodiscard]] u8 divider() const;

    void reset_divider();

    [[nodiscard]] u8 counter() const;

    void counter(u8 new_value);

    [[nodiscard]] u8 modulo() const;

    void modulo(u8 new_value);

    [[nodiscard]] u8 control() const;

    void control(u8 new_value);

private:
    static constexpr unsigned int s_timer_enabled_bit = 2;

    u8 m_divider { 0 };
    u8 m_counter { 0 };
    u8 m_modulo { 0 };
    bool m_is_running { false };
    TimerClockSpeed m_timer_clock_speed { TimerClockSpeed::_4096Hz };
};

}
