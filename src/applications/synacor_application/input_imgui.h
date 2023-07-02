#pragma once

#include "interfaces/input.h"
#include <SDL_scancode.h>

namespace emu::applications::synacor {
class GuiIo;
}

namespace emu::applications::synacor {

class InputImgui : public Input {
public:
    void read(GuiIo& gui_io) override;

    void read_debug_only(GuiIo& gui_io) override;

private:
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;
    static constexpr SDL_Scancode s_step_instruction = SDL_SCANCODE_F7;
    static constexpr SDL_Scancode s_continue_running = SDL_SCANCODE_F9;
};
}
