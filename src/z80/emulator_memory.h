#ifndef MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H
#define MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H

#include <cstdint>
#include <memory>
#include <vector>
#include "z80/interfaces/memory_observer.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

    class EmulatorMemory {
    public:
        void add(const std::vector<u8> &to_add);

        void add_memory_observer(MemoryObserver &observer);

        void remove_memory_observer(MemoryObserver *observer);

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

        std::vector<MemoryObserver *> m_memory_observers;

        void notify_memory_observers_about_memory_update(u16 address);
    };
}

#endif //MIKA_EMULATORS_Z80_EMULATOR_MEMORY_H
