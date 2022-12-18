#include "running_state.h"
#include "applications/lmc_application/gui_io.h"
#include "applications/lmc_application/interfaces/input.h"
#include "applications/lmc_application/states/state_context.h"
#include "applications/lmc_application/ui.h"
#include "chips/trivial/lmc/cpu.h"
#include "crosscutting/debugging/debugger.h"
#include "crosscutting/logging/logger.h"
#include "crosscutting/misc/governor.h"
#include <utility>

namespace emu::applications::lmc {

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

void RunningState::transition_to_run_awaiting_input()
{
    m_ctx->change_state(m_ctx->running_awaiting_input_state());
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
    if (m_ctx->m_governor.is_time_to_update()) {
        cycles = 0;
        while (cycles < static_cast<cyc>(s_cycles_per_tick) && !m_ctx->m_is_awaiting_input) {
            if (m_ctx->m_cpu->can_run_next_instruction()) {
                m_ctx->m_cpu->next_instruction();
            } else {
                if (m_ctx->m_is_only_run_once) {
                    transition_to_stop();
                    return;
                } else {
                    transition_to_pause();
                    return;
                }
            }
            ++cycles;
            if (m_ctx->m_is_in_debug_mode && m_ctx->m_debugger->has_breakpoint(m_ctx->m_cpu->pc())) {
                m_ctx->m_logger->info("Breakpoint hit: %03d", m_ctx->m_cpu->pc());
                transition_to_step();
                return;
            }
        }

        m_ctx->m_input->read(m_ctx->m_gui_io);

        if (m_ctx->m_gui_io.m_is_quitting) {
            m_ctx->m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return;
        } else if (m_ctx->m_gui_io.m_is_toggling_pause && m_ctx->m_is_awaiting_input) {
            m_ctx->m_gui_io.m_is_toggling_pause = false;
            transition_to_run_awaiting_input();
            return;
        } else if (m_ctx->m_gui_io.m_is_toggling_pause && !m_ctx->m_is_awaiting_input) {
            m_ctx->m_gui_io.m_is_toggling_pause = false;
            transition_to_run();
            return;
        } else if (m_ctx->m_is_awaiting_input) {
            transition_to_run_awaiting_input();
            return;
        }

        m_ctx->m_ui->update_screen(m_ctx->m_is_awaiting_input, s_game_window_subtitle);
    }
}
}
