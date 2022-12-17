#include "paused_state.h"
#include "applications/space_invaders/gui.h"
#include "applications/space_invaders/gui_io.h"
#include "applications/space_invaders/interfaces/input.h"
#include "applications/space_invaders/states/state_context.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include <utility>

namespace emu::applications::space_invaders {

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
        m_ctx->m_input->read(m_ctx->m_cpu_io, m_ctx->m_gui_io);
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
    }
}

std::vector<u8> PausedState::vram()
{
    return { m_ctx->m_memory.begin() + 0x2400, m_ctx->m_memory.begin() + 0x3fff + 1 };
}

}
