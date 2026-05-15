#pragma once

#include <functional>

namespace emu::misc {

class Governor {
public:
    Governor(
        long double limit,
        std::function<long double()> tick_retriever);

    auto is_time_to_update() -> bool;

private:
    long double m_last_tick;
    long double m_limit;
    std::function<long double()> m_tick_retriever;
};
}
