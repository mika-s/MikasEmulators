#include "running_awaiting_input_state.h"
#include "applications/lmc_application/gui_io.h"
#include "applications/lmc_application/interfaces/input.h"
#include "applications/lmc_application/states/state_context.h"
#include "applications/lmc_application/ui.h"
#include <utility>

namespace emu::misc {
class Governor;
}

namespace emu::applications::lmc {

using emu::misc::Governor;

RunningAwaitingInputState::RunningAwaitingInputState(std::shared_ptr<StateContext> state_context)
    : m_ctx(std::move(state_context))
{
}

bool RunningAwaitingInputState::is_exit_state()
{
    return false;
}

void RunningAwaitingInputState::transition_to_run()
{
    m_ctx->change_state(m_ctx->running_state());
}

void RunningAwaitingInputState::transition_to_run_awaiting_input()
{
}

void RunningAwaitingInputState::transition_to_pause()
{
    m_ctx->change_state(m_ctx->paused_state());
}

void RunningAwaitingInputState::transition_to_stop()
{
    m_ctx->change_state(m_ctx->stopped_state());
}

void RunningAwaitingInputState::transition_to_step()
{
    m_ctx->change_state(m_ctx->stepping_state());
}

void RunningAwaitingInputState::perform([[maybe_unused]] cyc& cycles)
{
    m_ctx->m_input->read(m_ctx->m_gui_io);

    if (m_ctx->m_gui_io.m_is_quitting) {
        m_ctx->m_gui_io.m_is_quitting = false;
        transition_to_stop();
        return;
    } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
        m_ctx->m_gui_io.m_is_toggling_pause = false;
        transition_to_pause();
        return;
    }

    m_ctx->m_ui->update_screen(m_ctx->m_is_awaiting_input, s_game_window_subtitle);

    if (!m_ctx->m_is_awaiting_input) {
        transition_to_run();
        return;
    }
}
}
