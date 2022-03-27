#ifndef MIKA_EMULATORS_8080_INTERFACES_GUI_OBSERVER_H
#define MIKA_EMULATORS_8080_INTERFACES_GUI_OBSERVER_H

namespace emu::cpu8080 {

    class GuiObserver {
    public:
        virtual ~GuiObserver() = default;

        virtual void run_status_changed(RunStatus new_status) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_INTERFACES_GUI_OBSERVER_H
