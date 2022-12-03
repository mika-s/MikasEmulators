#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_GUI_IMGUI_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_GUI_IMGUI_H

#include "chips/trivial/lmc/usings.h"
#include "crosscutting/gui/debugging_panes/cpu_info_pane.h"
#include "crosscutting/gui/debugging_panes/debug_log_pane.h"
#include "crosscutting/gui/debugging_panes/disassembly_pane.h"
#include "crosscutting/gui/debugging_panes/memory_editor_pane.h"
#include "crosscutting/gui/main_panes/terminal_pane.h"
#include "crosscutting/misc/run_status.h"
#include "ui.h"
#include <SDL_video.h>
#include <memory>
#include <vector>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::lmc {
    class GuiObserver;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::lmc {

    using emu::gui::CpuInfoPane;
    using emu::gui::DebugLogPane;
    using emu::gui::DisassemblyPane;
    using emu::gui::MemoryEditorPane;
    using emu::gui::TerminalPane;
    using emu::lmc::Address;
    using emu::lmc::Data;
    using emu::lmc::GuiObserver;
    using emu::misc::RunStatus;

    class GuiImgui : public Ui {

    public:
        GuiImgui();

        ~GuiImgui() override;

        void to_terminal() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(RunStatus run_status) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer<Address, Data> &debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;

    private:
        SDL_Window *m_win;
        SDL_GLContext m_gl_context;

        bool m_show_terminal;
        bool m_show_game_info;
        bool m_show_cpu_info;
        bool m_show_log;
        bool m_show_disassembly;
        bool m_show_memory_editor;

        bool m_is_in_debug_mode;

        std::vector<GuiObserver *> m_gui_observers;
        std::shared_ptr<Logger> m_logger;

        DebugLogPane m_log;
        DisassemblyPane m_disassembly;
        CpuInfoPane m_cpu_info;
        MemoryEditorPane m_memory_editor;
        TerminalPane m_terminal;

        void notify_gui_observers_about_run_status(RunStatus new_status);

        void notify_gui_observers_about_debug_mode();

        void init();

        void render(RunStatus run_status);

        void render_terminal_window(RunStatus run_status);

        void render_game_info_window();

        void render_cpu_info_window();

        void render_log_window();

        void render_disassembly_window();

        void render_memory_editor_window();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_GUI_IMGUI_H
