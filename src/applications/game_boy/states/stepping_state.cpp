#include "stepping_state.h"
#include "applications/game_boy/gui.h"
#include "applications/game_boy/gui_io.h"
#include "applications/game_boy/interfaces/input.h"
#include "applications/game_boy/interrupts.h"
#include "applications/game_boy/lcd.h"
#include "chips/lr35902/cpu.h"
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

auto SteppingState::is_exit_state() -> bool
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

    m_ctx->notify_interrupt_observers(VBLANK);

    m_ctx->m_input->read(m_ctx->m_gui_io, m_ctx->m_memory_mapped_io);
    if (m_ctx->m_gui_io.m_is_quitting) {
        m_ctx->m_gui_io.m_is_quitting = false;
        transition_to_stop();
        return;
    }

    if (m_ctx->m_gui_io.m_is_toggling_pause) {
        m_ctx->m_gui_io.m_is_toggling_pause = false;
        transition_to_run();
        return;
    }

    m_ctx->m_gui->update_screen(m_ctx->m_lcd->lcd_control(),
        tile_ram_block_1(), tile_ram_block_2(), tile_ram_block_3(),
        tile_map_1(), tile_map_2(), sprite_ram(), palette_ram(), s_game_window_subtitle);

    m_is_stepping_cycle = false;
}

auto SteppingState::await_input_and_update_debug() -> bool
{
    while (true) {
        m_ctx->m_input->read_debug_only(m_ctx->m_gui_io);

        if (m_ctx->m_gui_io.m_is_quitting) {
            m_ctx->m_gui_io.m_is_quitting = false;
            transition_to_stop();
            return true;
        }

        if (m_ctx->m_gui_io.m_is_toggling_pause) {
            m_ctx->m_gui_io.m_is_toggling_pause = false;
            transition_to_pause();
            return true;
        }

        if (m_ctx->m_gui_io.m_is_stepping_cycle) {
            m_ctx->m_gui_io.m_is_stepping_cycle = false;
            m_is_stepping_cycle = true;
            break;
        }

        if (m_ctx->m_gui_io.m_is_stepping_instruction) {
            m_ctx->m_gui_io.m_is_stepping_instruction = false;
            break;
        }

        if (m_ctx->m_gui_io.m_is_continuing_execution) {
            m_ctx->m_gui_io.m_is_continuing_execution = false;
            transition_to_run();
            return true;
        }

        m_ctx->m_gui->update_debug_only();
    }

    return false;
}

auto SteppingState::tile_ram_block_1() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x8000, m_ctx->m_memory.begin() + 0x87ff + 1 };
}

auto SteppingState::tile_ram_block_2() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x8800, m_ctx->m_memory.begin() + 0x8fff + 1 };
}

auto SteppingState::tile_ram_block_3() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x9000, m_ctx->m_memory.begin() + 0x97ff + 1 };
}

auto SteppingState::palette_ram() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x4400, m_ctx->m_memory.begin() + 0x47ff + 1 };
}

auto SteppingState::sprite_ram() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x4ff0, m_ctx->m_memory.begin() + 0x506f + 1 };
}

auto SteppingState::tile_map_1() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x9800, m_ctx->m_memory.begin() + 0x9bff + 1 };
}

auto SteppingState::tile_map_2() const -> std::vector<u8>
{
    return { m_ctx->m_memory.begin() + 0x9c00, m_ctx->m_memory.begin() + 0x9fff + 1 };
}

}
