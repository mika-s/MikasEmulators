#ifndef MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H

#include <cstdint>

namespace emu::util::byte {

    bool is_bit_set(std::uint8_t variable, unsigned int bit_position);

    void set_bit(std::uint8_t &variable, unsigned int bit_position);

    void unset_bit(std::uint8_t &variable, unsigned int bit_position);

    std::uint16_t to_u16(std::uint8_t farg, std::uint8_t sarg);

    std::uint8_t first_byte(std::uint16_t number);

    std::uint8_t second_byte(std::uint16_t number);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H
