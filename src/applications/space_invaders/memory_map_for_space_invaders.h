#ifndef MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_MEMORY_MAPPED_IO_FOR_SPACE_INVADERS_H
#define MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_MEMORY_MAPPED_IO_FOR_SPACE_INVADERS_H

#include "crosscutting/memory/memory_mapped_io.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include <cstddef>

namespace emu::memory {
    template<class A, class D>
    class EmulatorMemory;
}

namespace emu::applications::space_invaders {

    using emu::memory::EmulatorMemory;
    using emu::memory::MemoryMappedIo;
    using emu::util::byte::low_nibble;

    class MemoryMapForSpaceInvaders : public MemoryMappedIo<u16, u8> {
    public:
        explicit MemoryMapForSpaceInvaders(EmulatorMemory<u16, u8> &memory);

        void write(u16 address, u8 value) override;

        u8 read(u16 address) override;

    private:
        static constexpr std::size_t address_mask = 0x3fff;
        static constexpr u16 address_rom_end = 0x1fff;
        static constexpr u16 address_ram_end = 0x3fff;

        EmulatorMemory<u16, u8> &m_memory;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_SPACE_INVADERS_MEMORY_MAPPED_IO_FOR_SPACE_INVADERS_H
