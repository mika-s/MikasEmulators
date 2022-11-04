#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_SDL_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_SDL_H

#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <vector>

namespace emu::debugger {
    class DebugContainer;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::logging {
    class Logger;
}
namespace emu::z80 {
    class GuiObserver;
}

namespace emu::applications::pacman {

    class GuiSdl : public Gui {
    public:
        GuiSdl();

        ~GuiSdl() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(
                const std::vector<u8> &tile_ram,
                const std::vector<u8> &sprite_ram,
                const std::vector<u8> &palette_ram,
                RunStatus run_status,
                bool is_screen_flipped
        ) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer &debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;

        void toggle_tile_debug() override;

        void toggle_sprite_debug() override;

    private:
        SDL_Window *m_win;
        SDL_Renderer *m_rend;
        SDL_Texture *m_texture;

        std::vector<GuiObserver *> m_gui_observers;

        void notify_gui_observers(RunStatus new_status);

        void init();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_GUI_SDL_H
