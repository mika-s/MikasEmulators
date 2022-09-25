#ifndef MIKA_EMULATORS_Z80_INTERFACES_MEMORY_OBSERVER_H
#define MIKA_EMULATORS_Z80_INTERFACES_MEMORY_OBSERVER_H

#include "z80/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class MemoryObserver {
    public:
        virtual ~MemoryObserver() = default;

        virtual void memory_changed(u16 address) = 0;
    };
}

#endif //MIKA_EMULATORS_Z80_INTERFACES_MEMORY_OBSERVER_H
