#ifndef MIKA_EMULATORS_CHIPS_8080_EMULATORMEMORY_H
#define MIKA_EMULATORS_CHIPS_8080_EMULATORMEMORY_H

#include <cstdint>
#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::i8080 {
    class EmulatorMemory {

    public:
        void add(const std::vector<u8> &to_add);

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
    };
}

#endif //MIKA_EMULATORS_CHIPS_8080_EMULATORMEMORY_H
