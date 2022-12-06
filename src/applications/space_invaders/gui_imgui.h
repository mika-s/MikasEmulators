#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H

#include "crosscutting/gui/debugging_panes/cpu_info_pane.h"
#include "crosscutting/gui/debugging_panes/debug_log_pane.h"
#include "crosscutting/gui/debugging_panes/disassembly_pane.h"
#include "crosscutting/gui/debugging_panes/io_info_pane.h"
#include "crosscutting/gui/debugging_panes/memory_editor_pane.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_video.h>
#include <memory>
#include <vector>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
    template<class A>
    class Debugger;
}
namespace emu::i8080 {
    class GuiObserver;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::space_invaders {

    using emu::gui::CpuInfoPane;
    using emu::gui::DebugLogPane;
    using emu::gui::DisassemblyPane;
    using emu::gui::IoInfoPane;
    using emu::gui::MemoryEditorPane;
    using emu::i8080::GuiObserver;
    using emu::misc::RunStatus;

    class GuiImgui : public Gui {

    public:
        GuiImgui();

        ~GuiImgui() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(const std::vector<u8> &vram, RunStatus run_status) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger<u16>> debugger) override;

        void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8>> debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;

    private:
        SDL_Window *m_win;
        SDL_GLContext m_gl_context;
        u32 m_screen_texture;

        bool m_show_game;
        bool m_show_game_info;
        bool m_show_cpu_info;
        bool m_show_io_info;
        bool m_show_log;
        bool m_show_disassembly;
        bool m_show_memory_editor;
        bool m_show_demo;

        bool m_is_in_debug_mode;

        std::vector<GuiObserver *> m_gui_observers;
        std::shared_ptr<Logger> m_logger;

        DebugLogPane m_log;
        DisassemblyPane<u16, u8> m_disassembly;
        CpuInfoPane<u16, u8> m_cpu_info;
        IoInfoPane<u16, u8> m_io_info;
        MemoryEditorPane<u16, u8> m_memory_editor;

        void notify_gui_observers_about_run_status(RunStatus new_status);

        void notify_gui_observers_about_debug_mode();

        void init();

        void render(RunStatus run_status);

        void render_game_window(RunStatus run_status);

        void render_game_info_window();

        void render_cpu_info_window();

        void render_io_info_window();

        void render_log_window();

        void render_disassembly_window();

        void render_memory_editor_window();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_IMGUI_H
