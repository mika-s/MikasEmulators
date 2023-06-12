#include "paused_state.h"
#include "applications/game_boy/gui.h"
#include "applications/game_boy/gui_io.h"
#include "applications/game_boy/interfaces/input.h"
#include "applications/game_boy/lcd.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "state_context.h"
#include <utility>

namespace emu::applications::game_boy {

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
        m_ctx->m_gui->update_screen(m_ctx->m_lcd->lcd_control(),
            tile_ram_block_1(), tile_ram_block_2(), tile_ram_block_3(),
            tile_map_1(), tile_map_2(), sprite_ram(), palette_ram(), s_game_window_subtitle);
    }
}

std::vector<u8> PausedState::tile_ram_block_1()
{
    return { m_ctx->m_memory.begin() + 0x8000, m_ctx->m_memory.begin() + 0x87ff + 1 };
}

std::vector<u8> PausedState::tile_ram_block_2()
{
    return { m_ctx->m_memory.begin() + 0x8800, m_ctx->m_memory.begin() + 0x8fff + 1 };
}

std::vector<u8> PausedState::tile_ram_block_3()
{
    return { m_ctx->m_memory.begin() + 0x9000, m_ctx->m_memory.begin() + 0x97ff + 1 };
}

std::vector<u8> PausedState::palette_ram()
{
    return { m_ctx->m_memory.begin() + 0x4400, m_ctx->m_memory.begin() + 0x47ff + 1 };
}

std::vector<u8> PausedState::sprite_ram()
{
    return { m_ctx->m_memory.begin() + 0x4ff0, m_ctx->m_memory.begin() + 0x506f + 1 };
}

std::vector<u8> PausedState::tile_map_1()
{
    return { m_ctx->m_memory.begin() + 0x9800, m_ctx->m_memory.begin() + 0x9bff + 1 };
}

std::vector<u8> PausedState::tile_map_2()
{
    return { m_ctx->m_memory.begin() + 0x9c00, m_ctx->m_memory.begin() + 0x9fff + 1 };
}

}
