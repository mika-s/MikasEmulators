#ifndef MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DISASSEMBLY_PANE_H
#define MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DISASSEMBLY_PANE_H

#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::debugger {
    class Debugger;
}
namespace emu::logging {
    class Logger;
}

namespace emu::gui {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::logging::Logger;

    class DisassemblyPane {
    public:
        DisassemblyPane();

        void attach_debugger(std::shared_ptr<Debugger> debugger);

        void attach_debug_container(DebugContainer &debug_container);

        void attach_logger(std::shared_ptr<Logger> logger);

        void draw(const char *title, bool *p_open = nullptr);

    private:
        static constexpr int max_address_size = 9;
        static constexpr int address_base = 16;

        std::shared_ptr<Debugger> m_debugger;
        DebugContainer m_debug_container;
        bool m_is_debugger_set;
        bool m_is_debug_container_set;
        bool m_is_logger_set;
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
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_GUI_DEBUGGING_PANES_DISASSEMBLY_PANE_H
