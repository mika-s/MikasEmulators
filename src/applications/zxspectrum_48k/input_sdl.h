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

class InputSdl : public Input {
public:
    void read(GuiIo& gui_io) override;

    void read_debug_only(GuiIo& gui_io) override;

    void add_io_observer(KeyObserver& observer) override;

    void remove_io_observer(KeyObserver* observer) override;

private:
    static constexpr SDL_Scancode s_mute = SDL_SCANCODE_M;
    static constexpr SDL_Scancode s_pause = SDL_SCANCODE_PAUSE;

    std::vector<KeyObserver*> m_io_observers;

    void notify_io_observers(KeyRequest request);
};
}
