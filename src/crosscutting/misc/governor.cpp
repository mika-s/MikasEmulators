#include "governor.h"

namespace emu::misc {

    Governor::Governor(double gain, long double limit, std::function<u64()> tick_retriever)
            : m_gain(gain),
              m_last_tick(0),
              m_limit(limit),
              m_tick_retriever(std::move(tick_retriever)) {
    }

    bool Governor::is_time_to_update() {
        const u64 ticks = m_tick_retriever();
        bool should_update = ticks - m_last_tick >= m_limit;
        if (should_update) {
            m_last_tick = ticks;
        }

        return should_update;
    }
}
