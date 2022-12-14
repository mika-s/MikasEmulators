#include "paused_state.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "space_invaders/gui.h"
#include "space_invaders/gui_io.h"
#include "space_invaders/interfaces/input.h"
#include "space_invaders/interfaces/state_manager.h"
#include <utility>

namespace emu::applications::space_invaders {

PausedState::PausedState(
    StateManager& state_manager,
    CpuIo& cpu_io,
    GuiIo& gui_io,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    EmulatorMemory<u16, u8>& memory,
    Governor& governor)
    : m_state_manager(state_manager)
    , m_cpu_io(cpu_io)
    , m_gui_io(gui_io)
    , m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_memory(memory)
    , m_governor(governor)
{
}

bool PausedState::is_exit_state()
{
    return false;
}

void PausedState::transition_to_run()
{
    m_state_manager.change_state(m_state_manager.running_state());
}

void PausedState::transition_to_pause()
{
}

void PausedState::transition_to_stop()
{
    m_state_manager.change_state(m_state_manager.stopped_state());
}

void PausedState::transition_to_step()
{
}

void PausedState::perform([[maybe_unused]] cyc& cycles)
{
    if (m_governor.is_time_to_update()) {
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
    }
}

std::vector<u8> PausedState::vram()
{
    return { m_memory.begin() + 0x2400, m_memory.begin() + 0x3fff + 1 };
}

}
