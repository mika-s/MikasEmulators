#pragma once

#include "chips/trivial/lmc/interfaces/in_observer.h"
#include "chips/trivial/lmc/interfaces/out_observer.h"
#include "chips/trivial/lmc/interfaces/ui_observer.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include "interfaces/io_observer.h"
#include "io_request.h"
#include "terminal_input_state.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::lmc {
class Ui;
}
namespace emu::applications::lmc {
class Input;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::debugger {
template<class A, std::size_t B>
class DisassembledLine;
}
namespace emu::lmc {
class Cpu;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::lmc {

using emu::applications::lmc::IoObserver;
using emu::debugger::DebugContainer;
using emu::debugger::Debugger;
using emu::debugger::DisassembledLine;
using emu::lmc::Address;
using emu::lmc::Cpu;
using emu::lmc::Data;
using emu::lmc::InObserver;
using emu::lmc::OutObserver;
using emu::lmc::OutType;
using emu::lmc::UiObserver;
using emu::logging::Logger;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;
using emu::misc::RunStatus;
using emu::misc::Session;
using emu::misc::UInteger;

class LmcApplicationSession
    : public Session
    , public UiObserver
    , public OutObserver
    , public InObserver
    , public IoObserver {
public:
    LmcApplicationSession(
        bool is_only_run_once,
        const RunStatus startup_runstatus,
        std::shared_ptr<Ui> gui,
        std::shared_ptr<Input> input,
        std::string loaded_file,
        std::string file_content,
        EmulatorMemory<Address, Data> memory);

    ~LmcApplicationSession() override;

    void run() override;

    void pause() override;

    void stop() override;

    void run_status_changed(RunStatus new_status) override;

    void debug_mode_changed(bool is_in_debug_mode) override;

    void source_code_changed(std::string const& source) override;

    void assemble_and_load_request() override;

    void input_from_terminal(Data input) override;

    void out_changed(Data acc_reg, OutType out_type) override;

    void in_requested() override;

    void io_changed(IoRequest request) override;

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
    bool m_is_stepping_instruction { false };
    bool m_is_stepping_cycle { false };
    bool m_is_continuing_execution { false };
    TerminalInputState m_terminal_input_state;
    RunStatus m_startup_runstatus;
    RunStatus m_run_status;

    std::shared_ptr<Ui> m_gui;
    std::shared_ptr<Input> m_input;
    std::unique_ptr<Cpu> m_cpu;
    EmulatorMemory<Address, Data> m_memory;
    std::string m_loaded_file;
    std::string m_file_content;
    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<Address, 10>> m_debugger;
    std::shared_ptr<DebugContainer<Address, Data, 10>> m_debug_container;

    Governor m_governor;

    void running(cyc& cycles);

    void pausing();

    void stepping(cyc& cycles);

    void await_input_and_update();

    void await_input_and_update_debug();

    void setup_cpu();

    void setup_debugging();

    std::vector<Data> memory();

    std::vector<DisassembledLine<Address, 10>> disassemble_program();
};
}
