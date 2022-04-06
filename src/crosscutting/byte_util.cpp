#include "byte_util.h"

namespace emu::util::byte {

    bool is_bit_set(std::uint8_t variable, unsigned int bit_position) {
        return (variable >> bit_position) & 1;
    }

    void set_bit(std::uint8_t &variable, unsigned int bit_position) {
        variable |= (1U << bit_position);
    }

    void unset_bit(std::uint8_t &variable, unsigned int bit_position) {
        variable &= ~(1U << bit_position);
    }

    std::uint16_t to_u16(std::uint8_t farg, std::uint8_t sarg) {
        return (farg << 8u) + sarg;
    }

    std::uint8_t first_byte(std::uint16_t number) {
        return (number >> (8 * 0)) & 0xff;
    }

    std::uint8_t second_byte(std::uint16_t number) {
        return (number >> (8 * 1)) & 0xff;
    }
}
