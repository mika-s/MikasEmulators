#ifndef MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H
#define MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H

#include "crosscutting/typedefs.h"

namespace emu::lmc {

    class OutObserver {
    public:
        virtual ~OutObserver() = default;

        virtual void out_changed(u16 acc_reg) = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H
