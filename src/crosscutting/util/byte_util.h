#ifndef MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H

#include "crosscutting/typedefs.h"

namespace emu::util::byte {

    bool is_bit_set(u8 variable, unsigned int bit_position);

    void set_bit(u8 &variable, unsigned int bit_position);

    void unset_bit(u8 &variable, unsigned int bit_position);

    u16 to_u16(u8 farg, u8 sarg);

    u8 first_byte(u16 number);

    u8 second_byte(u16 number);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_BYTE_UTIL_H
