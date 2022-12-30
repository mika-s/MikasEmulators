#pragma once

#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::pacman {
class GuiObserver;
}
namespace emu::debugger {
template<class A, class D, std::size_t B>
class DebugContainer;
}
namespace emu::debugger {
template<class A, std::size_t B>
class Debugger;
}
namespace emu::logging {
class Logger;
}

namespace emu::applications::pacman {

class GuiSdl : public Gui {
public:
    GuiSdl();

    ~GuiSdl() override;

    void add_gui_observer(GuiObserver& observer) override;

    void remove_gui_observer(GuiObserver* observer) override;

    void update_screen(
        std::vector<u8> const& tile_ram,
        std::vector<u8> const& sprite_ram,
        std::vector<u8> const& palette_ram,
        bool is_screen_flipped,
        std::string const& game_window_subtitle) override;

    void update_debug_only() override;

    void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

    void toggle_tile_debug() override;

    void toggle_sprite_debug() override;

private:
    SDL_Window* m_win { nullptr };
    SDL_Renderer* m_rend { nullptr };
    SDL_Texture* m_texture { nullptr };

    std::vector<GuiObserver*> m_gui_observers;

    void init();
};
}
