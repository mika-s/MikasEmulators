#pragma once

#include "chips/trivial/lmc/interfaces/in_observer.h"
#include "chips/trivial/lmc/interfaces/out_observer.h"
#include "chips/trivial/lmc/usings.h"
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

namespace emu::applications::lmc {
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
namespace emu::lmc {
class Cpu;
enum class OutType;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::lmc {

using emu::applications::lmc::UiObserver;
using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::lmc::Address;
using emu::lmc::Cpu;
using emu::lmc::Data;
using emu::lmc::InObserver;
using emu::lmc::OutObserver;
using emu::lmc::OutType;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::sdl_get_ticks_high_performance;
using emu::misc::Session;
using emu::misc::UInteger;

class LmcApplicationSession
    : public Session
    , public UiObserver
    , public OutObserver
    , public InObserver {
public:
    LmcApplicationSession(
        bool is_only_run_once,
        bool is_starting_paused,
        std::shared_ptr<Ui> gui,
        std::shared_ptr<Input> input,
        std::string loaded_file,
        std::string file_content,
        EmulatorMemory<Address, Data> memory);

    ~LmcApplicationSession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void gui_request(GuiRequest request) override;

    void out_changed(Data acc_reg, OutType out_type) override;

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
    std::shared_ptr<Debugger<Address, 10>> m_debugger;
    std::shared_ptr<DebugContainer<Address, Data, 10>> m_debug_container;

    Governor m_governor { Governor(s_tick_limit, sdl_get_ticks_high_performance) };

    std::shared_ptr<StateContext> m_state_context;

    void setup_cpu();

    void setup_debugging();

    std::vector<Data> memory();

    void assemble_and_load_request();

    void input_from_terminal(Data input);

    std::vector<DisassembledLine<Address, 10>> disassemble_program();
};
}
