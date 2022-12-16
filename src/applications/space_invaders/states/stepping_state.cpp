#include "stepping_state.h"
#include "chips/8080/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "space_invaders/gui.h"
#include "space_invaders/gui_io.h"
#include "space_invaders/interfaces/input.h"
#include "space_invaders/interfaces/state_manager.h"
#include <utility>

namespace emu::applications::space_invaders {

using emu::misc::Governor;

SteppingState::SteppingState(
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
    std::unordered_map<u8, u8>& outputs_during_cycle)
    : m_state_manager(state_manager)
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
{
}

bool SteppingState::is_exit_state()
{
    return false;
}

void SteppingState::transition_to_run()
{
    m_state_manager.change_state(m_state_manager.running_state());
}

void SteppingState::transition_to_pause()
{
    m_state_manager.change_state(m_state_manager.paused_state());
}

void SteppingState::transition_to_stop()
{
    m_state_manager.change_state(m_state_manager.stopped_state());
}

void SteppingState::transition_to_step()
{
}

void SteppingState::perform(cyc& cycles)
{
    m_outputs_during_cycle.clear();

    if (await_input_and_update_debug()) {
        return;
    }

    cycles = 0;
    while (cycles < static_cast<cyc>(s_cycles_per_tick / 2)) {
        cycles += m_cpu->next_instruction();
        if (!m_is_stepping_cycle) {
            if (await_input_and_update_debug()) {
                return;
            }
        }
    }

    if (m_cpu->is_inta()) {
        m_cpu->interrupt(s_rst_1_i8080);
    }

    cycles = 0;
    while (cycles < static_cast<cyc>(s_cycles_per_tick / 2)) {
        cycles += m_cpu->next_instruction();
        if (!m_is_stepping_cycle) {
            if (await_input_and_update_debug()) {
                return;
            }
        }
    }

    m_input->read(m_cpu_io, m_gui_io);
    if (m_gui_io.m_is_quitting) {
        m_gui_io.m_is_quitting = false;
        transition_to_stop();
        return;
    } else if (m_gui_io.m_is_toggling_pause) {
        m_gui_io.m_is_toggling_pause = false;
        transition_to_run();
        return;
    }

    m_gui->update_screen(vram(), s_game_window_subtitle);

    if (m_cpu->is_inta()) {
        m_cpu->interrupt(s_rst_2_i8080);
    }

    m_is_stepping_cycle = false;
}

bool SteppingState::await_input_and_update_debug()
{
    while (true) {
        m_input->read_debug_only(m_gui_io);

        if (m_gui_io.m_is_quitting) {
            m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return true;
        } else if (m_gui_io.m_is_toggling_pause) {
            m_gui_io.m_is_toggling_pause = false;
            transition_to_pause();
            return true;
        } else if (m_gui_io.m_is_stepping_cycle) {
            m_gui_io.m_is_stepping_cycle = false;
            m_is_stepping_cycle = true;
            break;
        } else if (m_gui_io.m_is_stepping_instruction) {
            m_gui_io.m_is_stepping_instruction = false;
            break;
        } else if (m_gui_io.m_is_continuing_execution) {
            m_gui_io.m_is_continuing_execution = false;
            transition_to_run();
            return true;
        }

        m_gui->update_debug_only();
    }

    return false;
}

std::vector<u8> SteppingState::vram()
{
    return { m_memory.begin() + 0x2400, m_memory.begin() + 0x3fff + 1 };
}

}
