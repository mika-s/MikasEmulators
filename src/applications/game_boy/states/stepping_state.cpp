#include "stepping_state.h"
#include "applications/game_boy/audio.h"
#include "applications/game_boy/gui.h"
#include "applications/game_boy/gui_io.h"
#include "applications/game_boy/interfaces/input.h"
#include "applications/game_boy/memory_mapped_io_for_pacman.h"
#include "chips/z80/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "state_context.h"
#include <unordered_map>
#include <utility>

namespace emu::misc {
class Governor;
}

namespace emu::applications::game_boy {

using emu::misc::Governor;

SteppingState::SteppingState(std::shared_ptr<StateContext> state_context)
    : m_ctx(std::move(state_context))
{
}

bool SteppingState::is_exit_state()
{
    return false;
}

void SteppingState::transition_to_run()
{
    m_ctx->change_state(m_ctx->running_state());
}

void SteppingState::transition_to_pause()
{
    m_ctx->change_state(m_ctx->paused_state());
}

void SteppingState::transition_to_stop()
{
    m_ctx->change_state(m_ctx->stopped_state());
}

void SteppingState::transition_to_step()
{
}

void SteppingState::perform(cyc& cycles)
{
    m_ctx->m_outputs_during_cycle.clear();

    if (await_input_and_update_debug()) {
        return;
    }

    cycles = 0;
    while (cycles < static_cast<cyc>(s_cycles_per_tick)) {
        cycles += m_ctx->m_cpu->next_instruction();
        if (!m_is_stepping_cycle) {
            if (await_input_and_update_debug()) {
                return;
            }
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
            transition_to_run();
            return;
        }
        m_ctx->m_gui->update_screen(tile_ram(), sprite_ram(), palette_ram(), m_ctx->m_memory_mapped_io->is_screen_flipped(), s_game_window_subtitle);
        m_ctx->m_audio->handle_sound(m_ctx->m_memory_mapped_io->is_sound_enabled(), m_ctx->m_memory_mapped_io->voices());
    }

    m_is_stepping_cycle = false;
}

bool SteppingState::await_input_and_update_debug()
{
    while (true) {
        m_ctx->m_input->read_debug_only(m_ctx->m_gui_io);

        if (m_ctx->m_gui_io.m_is_quitting) {
            m_ctx->m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return true;
        } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
            m_ctx->m_gui_io.m_is_toggling_pause = false;
            transition_to_pause();
            return true;
        } else if (m_ctx->m_gui_io.m_is_stepping_cycle) {
            m_ctx->m_gui_io.m_is_stepping_cycle = false;
            m_is_stepping_cycle = true;
            break;
        } else if (m_ctx->m_gui_io.m_is_stepping_instruction) {
            m_ctx->m_gui_io.m_is_stepping_instruction = false;
            break;
        } else if (m_ctx->m_gui_io.m_is_continuing_execution) {
            m_ctx->m_gui_io.m_is_continuing_execution = false;
            transition_to_run();
            return true;
        }

        m_ctx->m_gui->update_debug_only();
    }

    return false;
}

std::vector<u8> SteppingState::tile_ram()
{
    return { m_ctx->m_memory.begin() + 0x4000, m_ctx->m_memory.begin() + 0x43ff + 1 };
}

std::vector<u8> SteppingState::palette_ram()
{
    return { m_ctx->m_memory.begin() + 0x4400, m_ctx->m_memory.begin() + 0x47ff + 1 };
}

std::vector<u8> SteppingState::sprite_ram()
{
    return { m_ctx->m_memory.begin() + 0x4ff0, m_ctx->m_memory.begin() + 0x506f + 1 };
}

}
