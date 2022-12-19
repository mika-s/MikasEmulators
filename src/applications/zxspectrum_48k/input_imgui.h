#pragma once

#include "interfaces/input.h"
#include "key_request.h"
#include <SDL_scancode.h>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class GuiIo;
}
namespace emu::applications::zxspectrum_48k {
class KeyObserver;
}

namespace emu::applications::zxspectrum_48k {

class InputImgui : public Input {
public:
    void read(GuiIo& gui_io) override;

    void read_debug_only(GuiIo& gui_io) override;

    void add_io_observer(KeyObserver& observer) override;

    void remove_io_observer(KeyObserver* observer) override;

private:
    static constexpr SDL_Scancode s_mute = SDL_SCANCODE_M;
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;
    static constexpr SDL_Scancode s_step_instruction = SDL_SCANCODE_F7;
    static constexpr SDL_Scancode s_step_cycle = SDL_SCANCODE_F8;
    static constexpr SDL_Scancode s_continue_running = SDL_SCANCODE_F9;

    std::vector<KeyObserver*> m_io_observers;

    void notify_io_observers(KeyRequest request);
};
}
