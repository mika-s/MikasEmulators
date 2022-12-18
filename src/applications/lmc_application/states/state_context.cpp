#include "state_context.h"
#include "applications/lmc_application/ui.h"
#include "chips/trivial/lmc/usings.h"
#include <utility>

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

using emu::misc::Governor;

StateContext::StateContext(
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
    bool& is_in_debug_mode)
    : m_is_only_run_once(is_only_run_once)
    , m_is_awaiting_input(is_awaiting_input)
    , m_is_in_debug_mode(is_in_debug_mode)
    , m_gui_io(gui_io)
    , m_ui(std::move(ui))
    , m_input(std::move(input))
    , m_cpu(std::move(cpu))
    , m_memory(memory)
    , m_logger(std::move(logger))
    , m_debugger(std::move(debugger))
    , m_debug_container(std::move(debug_container))
    , m_governor(governor)
{
}

void StateContext::change_state(std::shared_ptr<State> new_state)
{
    m_current_state = std::move(new_state);
}

std::shared_ptr<State> StateContext::paused_state()
{
    return m_paused_state;
}

std::shared_ptr<State> StateContext::running_state()
{
    return m_running_state;
}

std::shared_ptr<State> StateContext::running_awaiting_input_state()
{
    return m_running_awaiting_input_state;
}

std::shared_ptr<State> StateContext::stepping_state()
{
    return m_stepping_state;
}

std::shared_ptr<State> StateContext::stopped_state()
{
    return m_stopped_state;
}

std::shared_ptr<State> StateContext::current_state()
{
    return m_current_state;
}

void StateContext::set_running_state(std::shared_ptr<State> state)
{
    m_running_state = std::move(state);
}

void StateContext::set_running_awaiting_input_state(std::shared_ptr<State> state)
{
    m_running_awaiting_input_state = std::move(state);
}

void StateContext::set_paused_state(std::shared_ptr<State> state)
{
    m_paused_state = std::move(state);
}

void StateContext::set_stepping_state(std::shared_ptr<State> state)
{
    m_stepping_state = std::move(state);
}

void StateContext::set_stopped_state(std::shared_ptr<State> state)
{
    m_stopped_state = std::move(state);
}

}
