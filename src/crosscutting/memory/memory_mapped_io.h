#ifndef MIKA_EMULATORS_CROSSCUTTING_MEMORY_MEMORY_MAPPED_IO_H
#define MIKA_EMULATORS_CROSSCUTTING_MEMORY_MEMORY_MAPPED_IO_H

#include "crosscutting/typedefs.h"

namespace emu::memory {

    template<class A, class D>
    class MemoryMappedIo {
    public:
        virtual ~MemoryMappedIo() = default;

        virtual u8 read(u16 address) = 0;

        virtual void write(u16 address, u8 value) = 0;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_MEMORY_MEMORY_MAPPED_IO_H
