#ifndef MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H
#define MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H

#include <functional>
#include <memory>
#include <vector>
#include "interfaces/memory_mapped_io.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class EmulatorMemory {
    public:
        EmulatorMemory();

        void add(const std::vector<u8> &to_add);

        void attach_memory_mapper(std::shared_ptr<MemoryMappedIo> memory_mapper);

        std::size_t size();

        EmulatorMemory slice(std::size_t from, std::size_t to);

        void write(u16 address, u8 value);

        u8 read(u16 address) const;

        void direct_write(u16 address, u8 value);

        u8 direct_read(u16 address) const;

        std::vector<u8>::iterator begin();

        std::vector<u8>::iterator end();

        [[nodiscard]] std::vector<u8>::const_iterator begin() const;

        [[nodiscard]] std::vector<u8>::const_iterator end() const;

    private:
        std::vector<u8> m_memory;
        std::shared_ptr<MemoryMappedIo> m_memory_mapper;
        bool m_memory_mapper_is_attached;
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H
