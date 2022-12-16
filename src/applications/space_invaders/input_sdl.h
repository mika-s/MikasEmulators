#pragma once

#include "crosscutting/misc/run_status.h"
#include "interfaces/input.h"
#include "io_request.h"
#include <SDL_scancode.h>
#include <vector>

namespace emu::applications::space_invaders {
class CpuIo;
class GuiIo;
}
namespace emu::applications::space_invaders {
class IoObserver;
}

namespace emu::applications::space_invaders {

using emu::misc::RunStatus;

class InputSdl : public Input {
public:
    void read(CpuIo& cpu_io, GuiIo& gui_io) override;

    void read_debug_only(GuiIo& gui_io) override;

    void add_io_observer(IoObserver& observer) override;

    void remove_io_observer(IoObserver* observer) override;

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

    std::vector<IoObserver*> m_io_observers;

    void notify_io_observers(IoRequest request);
};
}
