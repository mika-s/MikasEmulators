#pragma once

#include "applications/lmc_application/ui.h"
#include "chips/trivial/lmc/usings.h"
#include <cstddef>
#include <memory>

namespace emu::applications::lmc {
class GuiIo;
class Input;
class State;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
template<class A, std::size_t B>
class Debugger;
}
namespace emu::lmc {
class Cpu;
}
namespace emu::logging {
class Logger;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}
namespace emu::misc {
class Governor;
}

namespace emu::applications::lmc {

using emu::lmc::Cpu;
using emu::memory::EmulatorMemory;
using emu::misc::Governor;

class StateContext {
public:
    explicit StateContext(
        GuiIo& gui_io,
        std::shared_ptr<Ui> ui,
        std::shared_ptr<Input> input,
        std::shared_ptr<Cpu> cpu,
        EmulatorMemory<Address, Data>& memory,
        std::shared_ptr<Logger> logger,
        std::shared_ptr<Debugger<Address, 10>> debugger,
        std::shared_ptr<DebugContainer<Address, Data, 10>> debug_container,
        Governor& governor,
        bool& is_only_run_once,
        bool& is_awaiting_input,
        bool& is_in_debug_mode);

    bool& m_is_only_run_once;
    bool& m_is_awaiting_input;
    bool& m_is_in_debug_mode;

    GuiIo& m_gui_io;
    std::shared_ptr<Ui> m_ui;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<Cpu> m_cpu;

    EmulatorMemory<Address, Data>& m_memory;

    std::shared_ptr<Logger> m_logger;
    std::shared_ptr<Debugger<Address, 10>> m_debugger;
    std::shared_ptr<DebugContainer<Address, Data, 10>> m_debug_container;

    Governor& m_governor;

    void change_state(std::shared_ptr<State> new_state);

    std::shared_ptr<State> paused_state();

    std::shared_ptr<State> running_state();

    std::shared_ptr<State> running_awaiting_input_state();

    std::shared_ptr<State> stepping_state();

    std::shared_ptr<State> stopped_state();

    std::shared_ptr<State> current_state();

    void set_running_state(std::shared_ptr<State> state);

    void set_running_awaiting_input_state(std::shared_ptr<State> state);

    void set_paused_state(std::shared_ptr<State> state);

    void set_stepping_state(std::shared_ptr<State> state);

    void set_stopped_state(std::shared_ptr<State> state);

private:
    std::shared_ptr<State> m_current_state;
    std::shared_ptr<State> m_running_state;
    std::shared_ptr<State> m_running_awaiting_input_state;
    std::shared_ptr<State> m_paused_state;
    std::shared_ptr<State> m_stepping_state;
    std::shared_ptr<State> m_stopped_state;
};
}
