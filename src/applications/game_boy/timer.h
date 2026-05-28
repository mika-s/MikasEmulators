#pragma once

#include "applications/game_boy/interrupts.h"
#include "crosscutting/typedefs.h"
#include <vector>

namespace emu::applications::game_boy {
class InterruptObserver;
}

namespace emu::applications::game_boy {

enum class TimerClockSpeed {
    _4096Hz,
    _262144Hz,
    _65536Hz,
    _16384Hz
};

class Timer {
public:
    void update(cyc cycles);

    [[nodiscard]] auto divider() const -> u8;

    void reset_divider();

    [[nodiscard]] auto counter() const -> u8;

    void counter(u8 new_value);

    [[nodiscard]] auto modulo() const -> u8;

    void modulo(u8 new_value);

    [[nodiscard]] auto control() const -> u8;

    void control(u8 new_value);

    void add_interrupt_observer(InterruptObserver& observer);

    void remove_interrupt_observer(InterruptObserver* observer);

private:
    static constexpr unsigned int s_timer_enabled_bit = 2;

    u8 m_divider { 0 };
    u8 m_counter { 0 };
    u8 m_modulo { 0 };
    bool m_is_running { false };
    TimerClockSpeed m_timer_clock_speed { TimerClockSpeed::_4096Hz };

    int m_internal_counter { 0 };
    int m_internal_divider_counter { 0 };

    std::vector<InterruptObserver*> m_interrupt_observers;

    void notify_interrupt_observers(Interrupts interrupt);
};

}
