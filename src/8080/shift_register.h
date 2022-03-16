#ifndef MIKA_EMULATORS_8080_SHIFT_REGISTER_H
#define MIKA_EMULATORS_8080_SHIFT_REGISTER_H

#include <cstdint>

namespace emu::cpu8080 {
    class ShiftRegister {

    public:

        ShiftRegister();

        void change_offset(std::uint8_t new_offset);

        void shift(std::uint8_t shift_value);

        [[nodiscard]] std::uint8_t read() const;

    private:
        std::uint16_t value;

        std::uint8_t offset;
    };
}

#endif //MIKA_EMULATORS_8080_SHIFT_REGISTER_H
