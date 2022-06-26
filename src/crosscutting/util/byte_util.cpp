#include "byte_util.h"

namespace emu::util::byte {

    bool is_bit_set(u8 variable, unsigned int bit_position) {
        return (variable >> bit_position) & 1;
    }

    void set_bit(u8 &variable, unsigned int bit_position) {
        variable |= (1U << bit_position);
    }

    void unset_bit(u8 &variable, unsigned int bit_position) {
        variable &= ~(1U << bit_position);
    }

    u16 to_u16(u8 farg, u8 sarg) {
        return (farg << 8u) + sarg;
    }

    u8 first_byte(u16 number) {
        return (number >> (8 * 0)) & 0xff;
    }

    u8 second_byte(u16 number) {
        return (number >> (8 * 1)) & 0xff;
    }
}
