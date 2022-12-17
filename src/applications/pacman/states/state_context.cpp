#include "state_context.h"
#include "pacman/gui.h"
#include <utility>

namespace emu::applications::pacman {
class State;
}

namespace emu::applications::pacman {

using emu::misc::Governor;

StateContext::StateContext(
    GuiIo& gui_io,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    std::shared_ptr<Audio> audio,
    std::shared_ptr<Cpu> cpu,
    EmulatorMemory<u16, u8>& memory,
    std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
    u8& vblank_interrupt_return,
    std::shared_ptr<Logger> logger,
    std::shared_ptr<Debugger<u16, 16>> debugger,
    std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container,
    std::unordered_map<u8, u8>& outputs_during_cycle,
    Governor& governor,
    bool& is_in_debug_mode)
    : m_is_in_debug_mode(is_in_debug_mode)
    , m_gui_io(gui_io)
    , m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_audio(std::move(audio))
    , m_cpu(std::move(cpu))
    , m_memory(memory)
    , m_memory_mapped_io(std::move(memory_mapped_io))
    , m_vblank_interrupt_return(vblank_interrupt_return)
    , m_logger(std::move(logger))
    , m_debugger(std::move(debugger))
    , m_debug_container(std::move(debug_container))
    , m_outputs_during_cycle(outputs_during_cycle)
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
