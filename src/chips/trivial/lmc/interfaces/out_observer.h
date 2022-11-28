#ifndef MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H
#define MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H

#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"

namespace emu::lmc {

    using emu::misc::UInteger;

    class OutObserver {
    public:
        virtual ~OutObserver() = default;

        virtual void out_changed(Data acc_reg, OutType out_type) = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_INTERFACES_OUT_OBSERVER_H
