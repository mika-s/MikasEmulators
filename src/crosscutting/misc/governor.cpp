#include "governor.h"

namespace emu::misc {

    Governor::Governor(
            long double limit,
            std::function<long double()> tick_retriever
    )
            : m_last_tick(0),
              m_limit(limit),
              m_tick_retriever(std::move(tick_retriever)) {
    }

    bool Governor::is_time_to_update() {
        const long double ticks = m_tick_retriever();
        const long double current_ms = ticks - m_last_tick;

        const bool should_update = current_ms >= m_limit;

        if (should_update) {
            m_last_tick = ticks;
        }

        return should_update;
    }
}
