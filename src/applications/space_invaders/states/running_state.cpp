#include "running_state.h"
#include "chips/8080/cpu.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/typedefs.h"
#include "space_invaders/gui.h"
#include "space_invaders/gui_io.h"
#include "space_invaders/interfaces/input.h"
#include "space_invaders/interfaces/state_manager.h"
#include <utility>

namespace emu::applications::space_invaders {
class CpuIo;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}

namespace emu::applications::space_invaders {

using emu::misc::Governor;

RunningState::RunningState(
    StateManager& state_manager,
    CpuIo& cpu_io,
    GuiIo& gui_io,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    std::shared_ptr<Cpu> cpu,
    EmulatorMemory<u16, u8>& memory,
    std::shared_ptr<Logger> logger,
    std::shared_ptr<Debugger<u16, 16>> debugger,
    std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container,
    std::unordered_map<u8, u8>& outputs_during_cycle,
    Governor& governor,
    bool& is_in_debug_mode)
    : m_state_manager(state_manager)
    , m_is_in_debug_mode(is_in_debug_mode)
    , m_cpu_io(cpu_io)
    , m_gui_io(gui_io)
    , m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_cpu(std::move(cpu))
    , m_memory(memory)
    , m_logger(std::move(logger))
    , m_debugger(std::move(debugger))
    , m_debug_container(std::move(debug_container))
    , m_outputs_during_cycle(outputs_during_cycle)
    , m_governor(governor)
{
}

bool RunningState::is_exit_state()
{
    return false;
}

void RunningState::transition_to_run()
{
}

void RunningState::transition_to_pause()
{
    m_state_manager.change_state(m_state_manager.paused_state());
}

void RunningState::transition_to_stop()
{
    m_state_manager.change_state(m_state_manager.stopped_state());
}

void RunningState::transition_to_step()
{
    m_state_manager.change_state(m_state_manager.stepping_state());
}

void RunningState::perform(cyc& cycles)
{
    m_outputs_during_cycle.clear();

    if (m_governor.is_time_to_update()) {
        cycles = 0;
        while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
            cycles += m_cpu->next_instruction();
            if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                transition_to_step();
                return;
            }
        }

        if (m_cpu->is_inta()) {
            m_cpu->interrupt(rst_1_i8080);
        }

        cycles = 0;
        while (cycles < static_cast<cyc>(cycles_per_tick / 2)) {
            cycles += m_cpu->next_instruction();
            if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                transition_to_step();
                return;
            }
        }

        m_input->read(m_cpu_io, m_gui_io);
        if (m_gui_io.m_is_quitting) {
            m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return;
        } else if (m_gui_io.m_is_toggling_pause) {
            m_gui_io.m_is_toggling_pause = false;
            transition_to_pause();
            return;
        }

        m_gui->update_screen(vram(), s_game_window_subtitle);

        if (m_cpu->is_inta()) {
            m_cpu->interrupt(rst_2_i8080);
        }
    }
}

std::vector<u8> RunningState::vram()
{
    return { m_memory.begin() + 0x2400, m_memory.begin() + 0x3fff + 1 };
}

}
