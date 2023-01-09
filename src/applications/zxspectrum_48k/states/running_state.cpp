#include "running_state.h"
#include "applications/zxspectrum_48k/cpu_io.h"
#include "applications/zxspectrum_48k/gui.h"
#include "applications/zxspectrum_48k/gui_io.h"
#include "applications/zxspectrum_48k/interfaces/input.h"
#include "applications/zxspectrum_48k/states/state_context.h"
#include "chips/z80/cpu.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/typedefs.h"
#include <unordered_map>
#include <utility>

namespace emu::applications::zxspectrum_48k {

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

        m_ctx->m_input->read(m_ctx->m_cpu_io, m_ctx->m_gui_io);
        if (m_ctx->m_gui_io.m_is_quitting) {
            m_ctx->m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return;
        } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
            m_ctx->m_gui_io.m_is_toggling_pause = false;
            transition_to_pause();
            return;
        }

        m_ctx->m_gui->update_screen(vram(), color_ram(), m_ctx->m_cpu_io.border_color(), s_game_window_subtitle);

        if (m_ctx->m_cpu->is_inta()) {
            m_ctx->m_cpu->interrupt(s_rst_7_z80);
        }
    }
}

std::vector<u8> RunningState::vram()
{
    return { m_ctx->m_memory.begin() + 0x4000, m_ctx->m_memory.begin() + 0x57ff + 1 };
}

std::vector<u8> RunningState::color_ram()
{
    return { m_ctx->m_memory.begin() + 0x5800, m_ctx->m_memory.begin() + 0x5aff + 1 };
}

}
