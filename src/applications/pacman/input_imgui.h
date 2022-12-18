#pragma once

#include "interfaces/input.h"
#include "key_request.h"
#include <SDL_scancode.h>
#include <memory>
#include <vector>

namespace emu::applications::pacman {
class GuiIo;
class KeyObserver;
class MemoryMappedIoForPacman;
}

namespace emu::applications::pacman {

class InputImgui : public Input {
public:
    void read(GuiIo& gui_io, std::shared_ptr<MemoryMappedIoForPacman> memory_mapped_io) override;

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

    static constexpr SDL_Scancode s_credit = SDL_SCANCODE_X;
    static constexpr SDL_Scancode s_insert_coin_p1 = SDL_SCANCODE_C;
    static constexpr SDL_Scancode s_insert_coin_p2 = SDL_SCANCODE_V;

    static constexpr SDL_Scancode s_p1_start = SDL_SCANCODE_RETURN;
    static constexpr SDL_Scancode s_p1_up = SDL_SCANCODE_W;
    static constexpr SDL_Scancode s_p1_down = SDL_SCANCODE_S;
    static constexpr SDL_Scancode s_p1_left = SDL_SCANCODE_A;
    static constexpr SDL_Scancode s_p1_right = SDL_SCANCODE_D;

    static constexpr SDL_Scancode s_p2_start = SDL_SCANCODE_RSHIFT;
    static constexpr SDL_Scancode s_p2_up = SDL_SCANCODE_UP;
    static constexpr SDL_Scancode s_p2_down = SDL_SCANCODE_DOWN;
    static constexpr SDL_Scancode s_p2_left = SDL_SCANCODE_LEFT;
    static constexpr SDL_Scancode s_p2_right = SDL_SCANCODE_RIGHT;

    std::vector<KeyObserver*> m_io_observers;

    void notify_io_observers(IoRequest request);
};
}
