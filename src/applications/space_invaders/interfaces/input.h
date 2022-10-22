#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H

#include "io_observer.h"
#include "applications/space_invaders/cpu_io.h"
#include "crosscutting/misc/run_status.h"

namespace emu::applications::space_invaders {

    using emu::misc::RunStatus;

    class Input {
    public:
        virtual ~Input() = default;

        virtual void read(RunStatus &run_status, CpuIo &cpu_io) = 0;

        virtual void read_debug_only(RunStatus &run_status) = 0;

        virtual void add_io_observer(IoObserver &observer) = 0;

        virtual void remove_io_observer(IoObserver *observer) = 0;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_INPUT_H