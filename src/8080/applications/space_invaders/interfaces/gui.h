#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H

#include "8080/interfaces/gui_observer.h"

namespace emu::cpu8080::applications::space_invaders {

    class Gui {
    public:
        virtual ~Gui() = default;

        virtual void add_gui_observer(GuiObserver &observer) = 0;

        virtual void remove_gui_observer(GuiObserver *observer) = 0;

        virtual void update_screen(const std::vector<std::uint8_t> &vram, RunStatus run_status) = 0;

        virtual void attach_debug_container(DebugContainer &debug_container) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_GUI_H
