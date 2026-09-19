#pragma once

#include <functional>

namespace emu::misc {

class Governor {
public:
    Governor(
        double limit,
        std::function<double()> tick_retriever);

    auto is_time_to_update() -> bool;

private:
    double m_last_tick;
    double m_limit;
    std::function<double()> m_tick_retriever;
};
}
