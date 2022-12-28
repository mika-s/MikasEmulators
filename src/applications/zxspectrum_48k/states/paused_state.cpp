#include "paused_state.h"
#include "applications/zxspectrum_48k/cpu_io.h"
#include "applications/zxspectrum_48k/gui.h"
#include "applications/zxspectrum_48k/gui_io.h"
#include "applications/zxspectrum_48k/interfaces/input.h"
#include "applications/zxspectrum_48k/states/state_context.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include <utility>

namespace emu::applications::zxspectrum_48k {

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

        m_ctx->m_gui->update_screen(vram(), color_ram(), m_ctx->m_cpu_io.border_color(), s_game_window_subtitle);
    }
}

std::vector<u8> PausedState::vram()
{
    return { m_ctx->m_memory.begin() + 0x4000, m_ctx->m_memory.begin() + 0x57ff + 1 };
}

std::vector<u8> PausedState::color_ram()
{
    return { m_ctx->m_memory.begin() + 0x5800, m_ctx->m_memory.begin() + 0x5aff + 1 };
}

}
