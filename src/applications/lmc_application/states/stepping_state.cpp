#include "stepping_state.h"
#include "applications/lmc_application/gui_io.h"
#include "applications/lmc_application/interfaces/input.h"
#include "applications/lmc_application/ui.h"
#include "chips/trivial/lmc/cpu.h"
#include "crosscutting/typedefs.h"
#include "state_context.h"
#include <utility>

namespace emu::misc {
class Governor;
}

namespace emu::applications::lmc {

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

void SteppingState::transition_to_run_awaiting_input()
{
    m_ctx->change_state(m_ctx->running_awaiting_input_state());
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

void SteppingState::perform([[maybe_unused]] cyc& cycles)
{
#ifdef __EMSCRIPTEN__
    if (await_input_and_update_debug()) {
        return;
    }
    if (!m_ctx->m_gui_io.m_is_stepping_instruction) {
        return;
    }
    m_ctx->m_gui_io.m_is_stepping_instruction = false;

    if (m_ctx->m_cpu->can_run_next_instruction()) {
        m_ctx->m_cpu->next_instruction();
    } else {
        transition_to_pause();
        return;
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

    m_ctx->m_ui->update_screen(m_ctx->m_is_awaiting_input, s_game_window_subtitle);
#else
    if (await_input_and_update_debug()) {
        return;
    }

    cycles = 0;
    while (cycles < static_cast<cyc>(s_cycles_per_tick)) {
        if (m_ctx->m_cpu->can_run_next_instruction()) {
            m_ctx->m_cpu->next_instruction();
        } else {
            transition_to_pause();
            return;
        }
        ++cycles;
        if (await_input_and_update_debug()) {
            return;
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

    m_ctx->m_ui->update_screen(m_ctx->m_is_awaiting_input, s_game_window_subtitle);
#endif
}

bool SteppingState::await_input_and_update_debug()
{
#ifdef __EMSCRIPTEN__
    m_ctx->m_input->read_debug_only(m_ctx->m_gui_io);

    if (m_ctx->m_gui_io.m_is_quitting) {
        m_ctx->m_gui_io.m_is_quitting = false;
        transition_to_stop();
        return true;
    } else if (m_ctx->m_gui_io.m_is_toggling_pause) {
        m_ctx->m_gui_io.m_is_toggling_pause = false;
        transition_to_pause();
        return true;
    } else if (m_ctx->m_gui_io.m_is_stepping_instruction) {
        return false;
    } else if (m_ctx->m_gui_io.m_is_continuing_execution) {
        m_ctx->m_gui_io.m_is_continuing_execution = false;
        transition_to_run();
        return true;
    }

    m_ctx->m_ui->update_debug_only(m_ctx->m_is_awaiting_input);

    return false;
#else
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
        } else if (m_ctx->m_gui_io.m_is_stepping_instruction) {
            m_ctx->m_gui_io.m_is_stepping_instruction = false;
            break;
        } else if (m_ctx->m_gui_io.m_is_continuing_execution) {
            m_ctx->m_gui_io.m_is_continuing_execution = false;
            transition_to_run();
            return true;
        }

        m_ctx->m_ui->update_debug_only(m_ctx->m_is_awaiting_input);
    }

    return false;
#endif
}
}
