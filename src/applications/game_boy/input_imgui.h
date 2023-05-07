#pragma once

#include "interfaces/input.h"
#include "key_request.h"
#include <SDL_scancode.h>
#include <memory>
#include <vector>

namespace emu::applications::game_boy {
class GuiIo;
class KeyObserver;
class MemoryMappedIoForGameBoy;
}

namespace emu::applications::game_boy {

class InputImgui : public Input {
public:
    void read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForGameBoy> memory_mapped_io) override;

    void read_debug_only(GuiIo& gui_io) override;

    void add_io_observer(KeyObserver& observer) override;

    void remove_io_observer(KeyObserver* observer) override;

private:
    static constexpr SDL_Scancode s_tile_debug = SDL_SCANCODE_PAGEUP;
    static constexpr SDL_Scancode s_sprite_debug = SDL_SCANCODE_PAGEDOWN;
    static constexpr SDL_Scancode s_mute = SDL_SCANCODE_M;
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;
    static constexpr SDL_Scancode s_step_instruction = SDL_SCANCODE_F7;
    static constexpr SDL_Scancode s_step_cycle = SDL_SCANCODE_F8;
    static constexpr SDL_Scancode s_continue_running = SDL_SCANCODE_F9;

    static constexpr SDL_Scancode s_p1_start = SDL_SCANCODE_RETURN;
    static constexpr SDL_Scancode s_p1_select = SDL_SCANCODE_RSHIFT;
    static constexpr SDL_Scancode s_up = SDL_SCANCODE_W;
    static constexpr SDL_Scancode s_down = SDL_SCANCODE_S;
    static constexpr SDL_Scancode s_left = SDL_SCANCODE_A;
    static constexpr SDL_Scancode s_right = SDL_SCANCODE_D;
    static constexpr SDL_Scancode s_a = SDL_SCANCODE_RALT;
    static constexpr SDL_Scancode s_b = SDL_SCANCODE_RCTRL;

    std::vector<KeyObserver*> m_io_observers;

    void notify_io_observers(IoRequest request);
};
}
