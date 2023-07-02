#pragma once

#include "chips/trivial/synacor/interfaces/in_observer.h"
#include "chips/trivial/synacor/interfaces/out_observer.h"
#include "chips/trivial/synacor/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/sdl_counter.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/misc/uinteger.h"
#include "gui_io.h"
#include "interfaces/ui_observer.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::synacor {
class Input;
class StateContext;
class Ui;
struct GuiRequest;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
template<class A, std::size_t B>
class DisassembledLine;
}
namespace emu::synacor {
class Cpu;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::synacor {

using emu::applications::synacor::UiObserver;
using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;
using emu::misc::UInteger;
using emu::synacor::Address;
using emu::synacor::Cpu;
using emu::synacor::Data;
using emu::synacor::InObserver;
using emu::synacor::OutObserver;

class SynacorApplicationSession
    : public Session
    , public UiObserver
    , public OutObserver
    , public InObserver {
public:
    SynacorApplicationSession(
        bool is_only_run_once,
        bool is_starting_paused,
        std::shared_ptr<Ui> gui,
        std::shared_ptr<Input> input,
        std::string loaded_file,
        std::string file_content,
        EmulatorMemory<Address, Data> memory);

    ~SynacorApplicationSession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void gui_request(GuiRequest request) override;

    void out_changed(Data acc_reg) override;

    void in_requested() override;

private:
    static constexpr unsigned int s_program_size = 100;

    // Game loop - begin
    static constexpr long double s_fps = 60.0L;
    static constexpr long double s_tick_limit = 1000.0L / s_fps;
    static constexpr int s_cycles_per_ms = 2000;
    static constexpr long double s_cycles_per_tick = s_cycles_per_ms * s_tick_limit;
    // Game loop - end

    bool m_is_only_run_once { false };
    bool m_is_in_debug_mode { false };
    bool m_is_awaiting_input { false };

    GuiIo m_gui_io;
    std::shared_ptr<Ui> m_ui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Cpu> m_cpu;
    EmulatorMemory<Address, Data> m_memory;
    std::string m_loaded_file;
    std::string m_file_content;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<Address, 16>> m_debugger;
    std::shared_ptr<DebugContainer<Address, Data, 16>> m_debug_container;

    Governor m_governor { Governor(s_tick_limit, sdl_get_ticks_high_performance) };

    std::shared_ptr<StateContext> m_state_context;

    void setup_cpu();

    void setup_debugging();

    std::vector<Data> memory();

    void assemble_and_load_request();

    void input_from_terminal(Data input);

    std::vector<DisassembledLine<Address, 16>> disassemble_program();
};
}
