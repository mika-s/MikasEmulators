#ifndef MIKA_EMULATORS_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H
#define MIKA_EMULATORS_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H

#include "applications/pacman/io_request.h"

namespace emu::applications::pacman {

    class IoObserver {
    public:
        virtual ~IoObserver() = default;

        virtual void io_changed(IoRequest request) = 0;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_PACMAN_INTERFACES_IO_OBSERVER_H
