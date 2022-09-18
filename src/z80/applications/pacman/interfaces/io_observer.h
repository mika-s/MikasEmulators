#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H

#include "z80/applications/pacman/io_request.h"

namespace emu::z80::applications::pacman {

    class IoObserver {
    public:
        virtual ~IoObserver() = default;

        virtual void io_changed(IoRequest request) = 0;
    };
}

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H
