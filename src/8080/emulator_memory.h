#ifndef MIKA_EMULATORS_8080_EMULATORMEMORY_H
#define MIKA_EMULATORS_8080_EMULATORMEMORY_H

#include <cstdint>
#include <vector>

namespace emu::cpu8080 {
    class EmulatorMemory {

    public:
        void add(const std::vector<std::uint8_t> &to_add);
        void add_link(std::uint16_t from, std::uint16_t to);
        std::size_t size();
        std::uint8_t &operator[](std::size_t d);
        const std::uint8_t &operator[](std::size_t d) const;

        std::vector<std::uint8_t>::iterator begin();

        std::vector<std::uint8_t>::iterator end();

        [[nodiscard]] std::vector<std::uint8_t>::const_iterator begin() const;

        [[nodiscard]] std::vector<std::uint8_t>::const_iterator end() const;

    private:
        std::vector<std::uint8_t> real_memory;
        std::vector<std::uint16_t> indices;
    };
}

#endif //MIKA_EMULATORS_8080_EMULATORMEMORY_H
