#ifndef MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H
#define MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H

#include "8080/applications/space_invaders/cpu_io.h"
#include "8080/applications/space_invaders/interfaces/io_observer.h"

namespace emu::cpu8080::applications::space_invaders {

    class Input {
    public:
        virtual ~Input() = default;

        virtual void read(RunStatus &run_status, CpuIo &cpu_io) = 0;

        virtual void read_debug_only(RunStatus &run_status) = 0;

        virtual void add_io_observer(IoObserver &observer) = 0;

        virtual void remove_io_observer(IoObserver *observer) = 0;
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H
