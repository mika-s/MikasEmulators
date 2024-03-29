#include "running_state.h"
#include "applications/pacman/audio.h"
#include "applications/pacman/gui.h"
#include "applications/pacman/gui_io.h"
#include "applications/pacman/interfaces/input.h"
#include "applications/pacman/memory_mapped_io_for_pacman.h"
#include "chips/z80/cpu.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/typedefs.h"
#include "state_context.h"
#include <unordered_map>
#include <utility>

namespace emu::applications::pacman {

using emu::misc::Governor;

RunningState::RunningState(std::shared_ptr<StateContext> state_context)
    : m_ctx(std::move(state_context))
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
    m_ctx->change_state(m_ctx->paused_state());
}

void RunningState::transition_to_stop()
{
    m_ctx->change_state(m_ctx->stopped_state());
}

void RunningState::transition_to_step()
{
    m_ctx->change_state(m_ctx->stepping_state());
}

void RunningState::perform(cyc& cycles)
{
    m_ctx->m_outputs_during_cycle.clear();

    if (m_ctx->m_governor.is_time_to_update()) {
        cycles = 0;
        while (cycles < static_cast<cyc>(s_cycles_per_tick)) {
            cycles += m_ctx->m_cpu->next_instruction();
            if (m_ctx->m_is_in_debug_mode && m_ctx->m_debugger->has_breakpoint(m_ctx->m_cpu->pc())) {
                m_ctx->m_logger->info("Breakpoint hit: 0x%04x", m_ctx->m_cpu->pc());
                transition_to_step();
                return;
            }
        }

        if (m_ctx->m_memory_mapped_io->is_interrupt_enabled()) {
            m_ctx->m_cpu->interrupt(m_ctx->m_vblank_interrupt_return);

            m_ctx->m_input->read(m_ctx->m_gui_io, m_ctx->m_memory_mapped_io);
            if (m_ctx->m_gui_io.m_is_quitting) {
                m_ctx->m_gui_io.m_is_quitting = false;
                transition_to_stop();
                return;
            } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
                m_ctx->m_gui_io.m_is_toggling_pause = false;
                transition_to_pause();
                return;
            }
            m_ctx->m_gui->update_screen(tile_ram(), sprite_ram(), palette_ram(), m_ctx->m_memory_mapped_io->is_screen_flipped(), s_game_window_subtitle);
            m_ctx->m_audio->handle_sound(m_ctx->m_memory_mapped_io->is_sound_enabled(), m_ctx->m_memory_mapped_io->voices());
        }
    }
}

std::vector<u8> RunningState::tile_ram()
{
    return { m_ctx->m_memory.begin() + 0x4000, m_ctx->m_memory.begin() + 0x43ff + 1 };
}

std::vector<u8> RunningState::palette_ram()
{
    return { m_ctx->m_memory.begin() + 0x4400, m_ctx->m_memory.begin() + 0x47ff + 1 };
}

std::vector<u8> RunningState::sprite_ram()
{
    return { m_ctx->m_memory.begin() + 0x4ff0, m_ctx->m_memory.begin() + 0x506f + 1 };
}

}
