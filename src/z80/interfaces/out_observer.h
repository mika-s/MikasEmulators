#ifndef MIKA_EMULATORS_Z80_INTERFACES_OUT_OBSERVER_H
#define MIKA_EMULATORS_Z80_INTERFACES_OUT_OBSERVER_H

#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class OutObserver {
    public:
        virtual ~OutObserver() = default;

        virtual void out_changed(u8 port) = 0;
    };
}

#endif //MIKA_EMULATORS_Z80_INTERFACES_OUT_OBSERVER_H
