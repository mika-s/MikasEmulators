#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_GUI_SDL_H

#include "crosscutting/misc/run_status.h"
#include "crosscutting/typedefs.h"
#include "gui.h"
#include <SDL_render.h>
#include <SDL_video.h>
#include <memory>
#include <vector>

namespace emu::debugger {
    template<class A, class D>
    class DebugContainer;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::i8080 {
    class GuiObserver;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::space_invaders {

    using emu::i8080::GuiObserver;
    using emu::misc::RunStatus;

    class GuiSdl : public Gui {
    public:
        GuiSdl();

        ~GuiSdl() override;

        void add_gui_observer(GuiObserver &observer) override;

        void remove_gui_observer(GuiObserver *observer) override;

        void update_screen(const std::vector<u8> &vram, RunStatus run_status) override;

        void update_debug_only() override;

        void attach_debugger(std::shared_ptr<Debugger> debugger) override;

        void attach_debug_container(DebugContainer<u16, u8> &debug_container) override;

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
