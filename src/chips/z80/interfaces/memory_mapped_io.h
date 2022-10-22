#ifndef MIKA_EMULATORS_CHIPS_Z80_INTERFACES_MEMORY_MAPPED_IO_H
#define MIKA_EMULATORS_CHIPS_Z80_INTERFACES_MEMORY_MAPPED_IO_H

#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class MemoryMappedIo {
    public:
        virtual ~MemoryMappedIo() = default;

        virtual u8 read(u16 address) = 0;

        virtual void write(u16 address, u8 value) = 0;
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_INTERFACES_MEMORY_MAPPED_IO_H
