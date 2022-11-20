#ifndef MIKA_EMULATORS_CHIPS_LMC_INTERFACES_IN_OBSERVER_H
#define MIKA_EMULATORS_CHIPS_LMC_INTERFACES_IN_OBSERVER_H

#include "crosscutting/typedefs.h"

namespace emu::lmc {

    class InObserver {
    public:
        virtual ~InObserver() = default;

        virtual void in_requested(u8 port) = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_INTERFACES_IN_OBSERVER_H
