#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_LMC_MEMORY_EDITOR_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_LMC_MEMORY_EDITOR_H

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/misc/uinteger.h"
#include <cstddef>

namespace emu::applications::lmc {

    using emu::debugger::DebugContainer;
    using emu::lmc::Address;
    using emu::lmc::Data;

    class LmcMemoryEditor {
    public:
        void draw(const char *title, bool *p_open = nullptr);

        void attach_debug_container(DebugContainer<Address, Data> &debug_container);

    private:
        static constexpr unsigned int max_addresses = 100;
        static constexpr unsigned int max_chars = 4;
        static constexpr std::size_t rows = 10;
        static constexpr std::size_t cols = 10;
        static constexpr float box_width = 50.0f;

        DebugContainer<Address, Data> m_debug_container;
        bool m_is_debug_container_set{false};

        char m_values[max_addresses][max_chars]; // NOLINT
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_LMC_MEMORY_EDITOR_H
