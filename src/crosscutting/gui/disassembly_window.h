#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H

#include <string>
#include <vector>
#include "8080/debug_container.h"

namespace emu::util::gui {

    class DisassemblyWindow {
    public:
        DisassemblyWindow();

        void attach_debug_container(emu::cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr int max_address_size = 9;
        static constexpr int address_base = 16;

        emu::cpu8080::DebugContainer m_debug_container;
        std::vector<std::string> m_content;
        ImVector<std::string> m_line_offsets;

        char m_address_to_goto[max_address_size];

        std::unordered_map<std::string, bool> m_breakpoints;

        bool m_is_following_pc;

        static std::uint16_t address_from_disassembly_line(std::string line);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
