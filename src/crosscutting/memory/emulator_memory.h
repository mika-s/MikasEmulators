#ifndef MIKA_EMULATORS_CROSSCUTTING_MEMORY_EMULATOR_MEMORY_H
#define MIKA_EMULATORS_CROSSCUTTING_MEMORY_EMULATOR_MEMORY_H

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <memory>
#include <vector>

namespace emu::memory {
    class MemoryMappedIo;
}

namespace emu::memory {

    class EmulatorMemory {
    public:
        EmulatorMemory();

        void add(const std::vector<u8> &to_add);

        void attach_memory_mapper(std::shared_ptr<MemoryMappedIo> memory_mapper);

        std::size_t size();

        EmulatorMemory slice(std::size_t from, std::size_t to);

        void write(u16 address, u8 value);

        [[nodiscard]] u8 read(u16 address) const;

        void direct_write(u16 address, u8 value);

        [[nodiscard]] u8 direct_read(u16 address) const;

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

#endif //MIKA_EMULATORS_CROSSCUTTING_MEMORY_EMULATOR_MEMORY_H
