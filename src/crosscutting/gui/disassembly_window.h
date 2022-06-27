#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H

#include <string>
#include <vector>
#include "8080/debug_container.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"

namespace emu::gui {

    using emu::debugger::Debugger;
    using emu::logging::Logger;

    class DisassemblyWindow {
    public:
        DisassemblyWindow();

        void attach_debugger(std::shared_ptr<Debugger> debugger);

        void attach_debug_container(i8080::DebugContainer &debug_container);

        void attach_logger(std::shared_ptr<Logger> logger);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr int max_address_size = 9;
        static constexpr int address_base = 16;

        std::shared_ptr<Debugger> m_debugger;
        i8080::DebugContainer m_debug_container;
        std::vector<std::string> m_content;
        std::shared_ptr<Logger> m_logger;

        char m_address_to_goto_str[max_address_size];
        u16 m_address_to_goto;
        u16 m_bp_address_to_goto;

        bool m_is_following_pc;
        bool m_is_following_pc_previous;
        bool m_is_going_to_pc;
        bool m_is_going_to_address;
        bool m_is_going_to_breakpoint;

        void reset_temp_state();

        void draw_menubar();

        void draw_buttons();

        void draw_addresses();

        static u16 address_from_disassembly_line(std::string line);
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DISASSEMBLY_WINDOW_H
