#pragma once

#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/gui/debugging_panes/cpu_info_pane.h"
#include "crosscutting/gui/debugging_panes/debug_log_pane.h"
#include "crosscutting/gui/debugging_panes/disassembly_pane.h"
#include "crosscutting/gui/main_panes/code_editor_pane.h"
#include "crosscutting/gui/main_panes/terminal_pane.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/uinteger.h"
#include "lmc_memory_editor.h"
#include "terminal_input_state.h"
#include "ui.h"
#include <SDL_video.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::lmc {
class UiObserver;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::lmc {

using emu::gui::CodeEditorPane;
using emu::gui::CodeEditorPaneObserver;
using emu::gui::CpuInfoPane;
using emu::gui::DebugLogPane;
using emu::gui::DisassemblyPane;
using emu::gui::TerminalPane;
using emu::gui::TerminalPaneObserver;
using emu::lmc::Address;
using emu::lmc::Data;
using emu::lmc::OutType;
using emu::lmc::UiObserver;
using emu::misc::RunStatus;

class GuiImgui : public Ui
    , public CodeEditorPaneObserver
    , public TerminalPaneObserver {

public:
    GuiImgui();

    ~GuiImgui() override;

    void to_terminal(Data acc_reg, OutType out_type) override;

    void from_terminal() override;

    void clear_terminal() override;

    void add_ui_observer(UiObserver& observer) override;

    void remove_ui_observer(UiObserver* observer) override;

    void update_screen(RunStatus run_status, TerminalInputState terminal_input_state) override;

    void update_debug_only(TerminalInputState terminal_input_state) override;

    void attach_debugger(std::shared_ptr<Debugger<Address, 10>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

    void source_code_changed(std::string const& source_code) override;

    void assemble_and_load_request() override;

    void input_sent(std::string const& input) override;

private:
    SDL_Window* m_win;
    SDL_GLContext m_gl_context;

    bool m_show_code_editor;
    bool m_show_terminal;
    bool m_show_game_info;
    bool m_show_cpu_info;
    bool m_show_log;
    bool m_show_disassembly;
    bool m_show_memory_editor;

    bool m_is_in_debug_mode;

    std::vector<UiObserver*> m_gui_observers;
    std::shared_ptr<Logger> m_logger;

    DebugLogPane m_log;
    DisassemblyPane<Address, Data, 10> m_disassembly;
    CpuInfoPane<Address, Data, 10> m_cpu_info;
    LmcMemoryEditor m_memory_editor;
    CodeEditorPane<Address, Data, 10> m_code_editor;
    TerminalPane m_terminal;

    std::vector<std::string> m_output;

    void notify_ui_observers_about_run_status(RunStatus new_status);

    void notify_ui_observers_about_debug_mode();

    void notify_ui_observers_about_source_code_change(std::string const& source_code);

    void notify_ui_observers_about_assemble_and_load_request();

    void notify_ui_observers_about_input_from_terminal(Data input);

    void init();

    void render(RunStatus run_status, TerminalInputState terminal_input_state);

    void render_code_editor();

    void render_terminal_window(RunStatus run_status, TerminalInputState terminal_input_state);

    void render_game_info_window();

    void render_cpu_info_window();

    void render_log_window();

    void render_disassembly_window();

    void render_memory_editor_window();
};
}
