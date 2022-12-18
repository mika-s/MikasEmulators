#include "paused_state.h"
#include "applications/lmc_application/gui_io.h"
#include "applications/lmc_application/interfaces/input.h"
#include "applications/lmc_application/states/state_context.h"
#include "applications/lmc_application/ui.h"
#include "crosscutting/misc/governor.h"
#include <utility>

namespace emu::applications::lmc {

PausedState::PausedState(std::shared_ptr<StateContext> state_context)
    : m_ctx(std::move(state_context))
{
}

bool PausedState::is_exit_state()
{
    return false;
}

void PausedState::transition_to_run()
{
    m_ctx->change_state(m_ctx->running_state());
}

void PausedState::transition_to_run_awaiting_input()
{
    m_ctx->change_state(m_ctx->running_awaiting_input_state());
}

void PausedState::transition_to_pause()
{
}

void PausedState::transition_to_stop()
{
    m_ctx->change_state(m_ctx->stopped_state());
}

void PausedState::transition_to_step()
{
}

void PausedState::perform([[maybe_unused]] cyc& cycles)
{
    if (m_ctx->m_governor.is_time_to_update()) {
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
        }

        m_ctx->m_ui->update_screen(m_ctx->m_is_awaiting_input, s_game_window_subtitle);
    }
}
}
