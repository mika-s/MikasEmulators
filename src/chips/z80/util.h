#ifndef MIKA_EMULATORS_CHIPS_Z80_UTIL_H
#define MIKA_EMULATORS_CHIPS_Z80_UTIL_H

#include "crosscutting/typedefs.h"

namespace emu::memory {
    class EmulatorMemory;
}

namespace emu::z80 {

    using emu::memory::EmulatorMemory;

    void set_bit_in_memory(EmulatorMemory &memory, u16 address, unsigned int bit_position);

    void unset_bit_in_memory(EmulatorMemory &memory, u16 address, unsigned int bit_position);
}

#endif //MIKA_EMULATORS_CHIPS_Z80_UTIL_H
