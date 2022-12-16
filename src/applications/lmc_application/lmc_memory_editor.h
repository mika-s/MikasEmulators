#pragma once

#include "chips/trivial/lmc/usings.h"
#include <cstddef>
#include <memory>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}

namespace emu::applications::lmc {

using emu::debugger::DebugContainer;
using emu::lmc::Address;
using emu::lmc::Data;

class LmcMemoryEditor {
public:
    void draw(char const* title, bool* p_open = nullptr);

    void attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container);

private:
    static constexpr unsigned int s_max_addresses = 100;
    static constexpr unsigned int s_max_chars = 4;
    static constexpr std::size_t s_rows = 10;
    static constexpr std::size_t s_cols = 10;
    static constexpr float s_box_width = 50.0f;

    std::shared_ptr<DebugContainer<Address, Data, 10>> m_debug_container;
    bool m_is_debug_container_set { false };

    char m_values[s_max_addresses][s_max_chars]; // NOLINT
};
}
