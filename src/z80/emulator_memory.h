#ifndef MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H
#define MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
#include "z80/interfaces/memory_observer.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class EmulatorMemory {
    public:
        EmulatorMemory();

        void add(const std::vector<u8> &to_add);

        void add_address_mask(std::size_t mask);

        std::size_t size();

        EmulatorMemory slice(int from, int to);

        u8 &operator[](std::size_t address);

        const u8 &operator[](std::size_t address) const;

        std::vector<u8>::iterator begin();

        std::vector<u8>::iterator end();

        [[nodiscard]] std::vector<u8>::const_iterator begin() const;

        [[nodiscard]] std::vector<u8>::const_iterator end() const;

    private:
        std::vector<u8> m_memory;
        std::size_t m_mask;
    };
}

#endif //MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H
