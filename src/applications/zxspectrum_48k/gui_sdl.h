#pragma once

#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::zxspectrum_48k {
class CpuIo;
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

namespace emu::applications::zxspectrum_48k {

using emu::applications::zxspectrum_48k::GuiObserver;

class GuiSdl : public Gui {
public:
    GuiSdl();

    ~GuiSdl() override;

    void add_gui_observer(GuiObserver& observer) override;

    void remove_gui_observer(GuiObserver* observer) override;

    void update_screen(
        std::vector<u8> const& vram,
        std::vector<u8> const& color_ram,
        u8 border_color,
        std::string const& game_window_subtitle) override;

    void update_debug_only() override;

    void attach_debugger(std::shared_ptr<Debugger<u16, 16>> debugger) override;

    void attach_debug_container(std::shared_ptr<DebugContainer<u16, u8, 16>> debug_container) override;

    void attach_cpu_io(CpuIo const* cpu_io) override;

    void attach_logger(std::shared_ptr<Logger> logger) override;

private:
    SDL_Window* m_win;
    SDL_Renderer* m_rend;
    SDL_Texture* m_texture;

    std::vector<GuiObserver*> m_gui_observers;

    void init();
};
}
