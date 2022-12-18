#pragma once

#include "interfaces/input.h"
#include "key_request.h"
#include <SDL_scancode.h>
#include <vector>

namespace emu::applications::space_invaders {
class CpuIo;
class GuiIo;
}
namespace emu::applications::space_invaders {
class KeyObserver;
}

namespace emu::applications::space_invaders {

class InputSdl : public Input {
public:
    void read(CpuIo& cpu_io, GuiIo& gui_io) override;

    void read_debug_only(GuiIo& gui_io) override;

    void add_io_observer(KeyObserver& observer) override;

    void remove_io_observer(KeyObserver* observer) override;

private:
    static constexpr SDL_Scancode s_mute = SDL_SCANCODE_M;
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;

    static constexpr SDL_Scancode s_insert_coin = SDL_SCANCODE_C;
    static constexpr SDL_Scancode s_tilt = SDL_SCANCODE_T;

    static constexpr SDL_Scancode s_p1_start = SDL_SCANCODE_RETURN;
    static constexpr SDL_Scancode s_p1_shoot = SDL_SCANCODE_W;
    static constexpr SDL_Scancode s_p1_left = SDL_SCANCODE_A;
    static constexpr SDL_Scancode s_p1_right = SDL_SCANCODE_D;

    static constexpr SDL_Scancode s_p2_start = SDL_SCANCODE_RSHIFT;
    static constexpr SDL_Scancode s_p2_shoot = SDL_SCANCODE_UP;
    static constexpr SDL_Scancode s_p2_left = SDL_SCANCODE_LEFT;
    static constexpr SDL_Scancode s_p2_right = SDL_SCANCODE_RIGHT;

    std::vector<KeyObserver*> m_io_observers;

    void notify_io_observers(KeyRequest request);
};
}
