#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INTERFACES_IO_OBSERVER_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INTERFACES_IO_OBSERVER_H

#include "applications/lmc_application/io_request.h"

namespace emu::applications::lmc {

    class IoObserver {
    public:
        virtual ~IoObserver() = default;

        virtual void io_changed(IoRequest request) = 0;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_INTERFACES_IO_OBSERVER_H
