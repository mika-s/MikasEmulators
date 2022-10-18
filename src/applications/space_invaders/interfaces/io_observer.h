#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_IO_OBSERVER_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_IO_OBSERVER_H

#include "applications/space_invaders/io_request.h"

namespace emu::applications::space_invaders {

    class IoObserver {
    public:
        virtual ~IoObserver() = default;

        virtual void io_changed(IoRequest request) = 0;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_INTERFACES_IO_OBSERVER_H
