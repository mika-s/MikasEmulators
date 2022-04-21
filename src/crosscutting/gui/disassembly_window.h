#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H

#include <string>
#include <vector>
#include "8080/debug_container.h"
#include "src/crosscutting/debugging/debugger.h"

namespace emu::util::gui {

    class DisassemblyWindow {
    public:
        DisassemblyWindow();

        void attach_debugger(std::shared_ptr<emu::util::debugger::Debugger> debugger);

        void attach_debug_container(emu::cpu8080::DebugContainer &debug_container);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr int max_address_size = 9;
        static constexpr int address_base = 16;

        std::shared_ptr<emu::util::debugger::Debugger> m_debugger;
        emu::cpu8080::DebugContainer m_debug_container;
        std::vector<std::string> m_content;

        char m_address_to_goto_str[max_address_size];
        std::uint16_t m_address_to_goto;
        std::uint16_t m_bp_address_to_goto;

        bool m_is_following_pc;
        bool m_is_going_to_pc;
        bool m_is_going_to_address;
        bool m_is_going_to_breakpoint;

        void reset_temp_state();
        void draw_menubar();
        void draw_buttons();
        void draw_addresses();

        static std::uint16_t address_from_disassembly_line(std::string line);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
