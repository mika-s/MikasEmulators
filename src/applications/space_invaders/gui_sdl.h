#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H

#include <memory>
#include <vector>
#include <SDL_video.h>
#include <SDL_render.h>
#include "gui.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/run_status.h"

namespace emu::applications::space_invaders {

    using emu::i8080::GuiObserver;
    using emu::misc::RunStatus;

    class GuiSdl: public Gui {
    public:
        GuiSdl();

        ~GuiSdl() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(const std::vector<u8> &vram, RunStatus run_status) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer &debug_container) override;

        void attach_logger(std::shared_ptr<Logger> logger) override;

    private:
        SDL_Window *m_win;
        SDL_Renderer *m_rend;
        SDL_Texture *m_texture;

        std::vector<GuiObserver *> m_gui_observers;

        void notify_gui_observers(RunStatus new_status);

        void init();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H
