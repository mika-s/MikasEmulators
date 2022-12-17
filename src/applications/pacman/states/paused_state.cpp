#include "paused_state.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "pacman/gui.h"
#include "pacman/gui_io.h"
#include "pacman/interfaces/input.h"
#include "pacman/interfaces/state_manager.h"
#include "pacman/memory_mapped_io_for_pacman.h"
#include <utility>

namespace emu::applications::pacman {

PausedState::PausedState(
    StateManager& state_manager,
    GuiIo& gui_io,
    std::shared_ptr<Gui> gui,
    std::shared_ptr<Input> input,
    EmulatorMemory<u16, u8>& memory,
    std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io,
    Governor& governor)
    : m_state_manager(state_manager)
    , m_gui_io(gui_io)
    , m_gui(std::move(gui))
    , m_input(std::move(input))
    , m_memory(memory)
    , m_memory_mapped_io(std::move(memory_mapped_io))
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
        m_input->read(m_gui_io, m_memory_mapped_io);
        if (m_gui_io.m_is_quitting) {
            m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return;
        } else if (m_gui_io.m_is_toggling_pause) {
            m_gui_io.m_is_toggling_pause = false;
            transition_to_run();
            return;
        }
        m_gui->update_screen(tile_ram(), sprite_ram(), palette_ram(), m_memory_mapped_io->is_screen_flipped(), s_game_window_subtitle);
    }
}

std::vector<u8> PausedState::tile_ram()
{
    return { m_memory.begin() + 0x4000, m_memory.begin() + 0x43ff + 1 };
}

std::vector<u8> PausedState::palette_ram()
{
    return { m_memory.begin() + 0x4400, m_memory.begin() + 0x47ff + 1 };
}

std::vector<u8> PausedState::sprite_ram()
{
    return { m_memory.begin() + 0x4ff0, m_memory.begin() + 0x506f + 1 };
}

}
