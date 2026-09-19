#include "governor.h"
#include <utility>

namespace emu::misc {

Governor::Governor(
    const double limit,
    std::function<double()> tick_retriever)
    : m_last_tick(0)
    , m_limit(limit)
    , m_tick_retriever(std::move(tick_retriever))
{
}

auto Governor::is_time_to_update() -> bool
{
    double const ticks = m_tick_retriever();
    double const current_ms = ticks - m_last_tick;

    bool const should_update = current_ms >= m_limit;

    if (should_update) {
        m_last_tick = ticks;
    }

    return should_update;
}
}
