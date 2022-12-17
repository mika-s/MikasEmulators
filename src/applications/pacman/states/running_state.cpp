#include "running_state.h"
#include "applications/pacman/audio.h"
#include "applications/pacman/gui.h"
#include "applications/pacman/gui_io.h"
#include "applications/pacman/interfaces/input.h"
#include "applications/pacman/interfaces/state_manager.h"
#include "applications/pacman/memory_mapped_io_for_pacman.h"
#include "chips/z80/cpu.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/typedefs.h"
#include <utility>

namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}

namespace emu::applications::pacman {

using emu::misc::Governor;

RunningState::RunningState(
    StateManager& state_manager,
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
    : m_state_manager(state_manager)
    , m_is_in_debug_mode(is_in_debug_mode)
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
        while (cycles < static_cast<cyc>(s_cycles_per_tick)) {
            cycles += m_cpu->next_instruction();
            if (m_is_in_debug_mode && m_debugger->has_breakpoint(m_cpu->pc())) {
                m_logger->info("Breakpoint hit: 0x%04x", m_cpu->pc());
                transition_to_step();
                return;
            }
        }

        if (m_memory_mapped_io->is_interrupt_enabled()) {
            m_cpu->interrupt(m_vblank_interrupt_return);

            m_input->read(m_gui_io, m_memory_mapped_io);
            if (m_gui_io.m_is_quitting) {
                m_gui_io.m_is_quitting = false;
                transition_to_stop();
                return;
            } else if (m_gui_io.m_is_toggling_pause) {
                m_gui_io.m_is_toggling_pause = false;
                transition_to_pause();
                return;
            }
            m_gui->update_screen(tile_ram(), sprite_ram(), palette_ram(), m_memory_mapped_io->is_screen_flipped(), s_game_window_subtitle);
            m_audio->handle_sound(m_memory_mapped_io->is_sound_enabled(), m_memory_mapped_io->voices());
        }
    }
}

std::vector<u8> RunningState::tile_ram()
{
    return { m_memory.begin() + 0x4000, m_memory.begin() + 0x43ff + 1 };
}

std::vector<u8> RunningState::palette_ram()
{
    return { m_memory.begin() + 0x4400, m_memory.begin() + 0x47ff + 1 };
}

std::vector<u8> RunningState::sprite_ram()
{
    return { m_memory.begin() + 0x4ff0, m_memory.begin() + 0x506f + 1 };
}

}
