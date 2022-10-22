#ifndef MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H
#define MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H

#include <functional>
#include <memory>
#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class EmulatorMemory {
    public:
        EmulatorMemory();

        void add(const std::vector<u8> &to_add);

        void add_address_mask(std::size_t mask);

        void attach_memory_mapper_for_write(
                std::function<void(EmulatorMemory &memory, u16 address, u8 value)> memory_mapper_for_write
        );

        void attach_memory_mapper_for_read(
                std::function<u8(const EmulatorMemory &memory, u16 address)> memory_mapper_for_read
        );

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
        std::size_t m_mask;

        std::function<void(EmulatorMemory &memory, u16 address, u8 value)> m_memory_mapper_for_write;
        std::function<u8(const EmulatorMemory &memory, u16 address)> m_memory_mapper_for_read;

        bool m_memory_mapper_for_write_is_set;
        bool m_memory_mapper_for_read_is_set;
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_EMULATOR_MEMORY_H
