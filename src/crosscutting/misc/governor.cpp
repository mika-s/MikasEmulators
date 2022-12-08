#include "governor.h"
#include <utility>

namespace emu::misc {

Governor::Governor(
    long double limit,
    std::function<long double()> tick_retriever)
    : m_last_tick(0)
    , m_limit(limit)
    , m_tick_retriever(std::move(tick_retriever))
{
}

bool Governor::is_time_to_update()
{
    long double const ticks = m_tick_retriever();
    long double const current_ms = ticks - m_last_tick;

    bool const should_update = current_ms >= m_limit;

    if (should_update) {
        m_last_tick = ticks;
    }

    return should_update;
}
}
