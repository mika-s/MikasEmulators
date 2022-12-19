#include "stepping_state.h"
#include "applications/zxspectrum_48k/gui.h"
#include "applications/zxspectrum_48k/gui_io.h"
#include "applications/zxspectrum_48k/interfaces/input.h"
#include "applications/zxspectrum_48k/states/state_context.h"
#include "chips/z80/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include <unordered_map>
#include <utility>

namespace emu::applications::zxspectrum_48k {

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

    m_ctx->m_input->read(m_ctx->m_gui_io);
    if (m_ctx->m_gui_io.m_is_quitting) {
        m_ctx->m_gui_io.m_is_quitting = false;
        transition_to_stop();
        return;
    } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
        m_ctx->m_gui_io.m_is_toggling_pause = false;
        transition_to_run();
        return;
    }

    m_ctx->m_gui->update_screen(vram(), s_game_window_subtitle);

    if (m_ctx->m_cpu->is_inta()) {
        m_ctx->m_cpu->interrupt(1);
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

std::vector<u8> SteppingState::vram()
{
    return { m_ctx->m_memory.begin() + 0x4000, m_ctx->m_memory.begin() + 0x57ff + 1 };
}

}
