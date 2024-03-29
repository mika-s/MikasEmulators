#pragma once

#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include <cstddef>

namespace emu::util::byte {

using emu::misc::UInteger;

bool is_bit_set(u8 variable, unsigned int bit_position);

template<std::size_t M>
bool is_bit_set(UInteger<M> variable, unsigned int bit_position)
{
    return (variable.underlying() >> bit_position) & 1;
}

void set_bit(u8& variable, unsigned int bit_position);

void unset_bit(u8& variable, unsigned int bit_position);

u16 to_u16(u8 farg, u8 sarg);

u32 to_u32(u8 fiarg, u8 sarg, u8 targ, u8 foarg);

u8 low_byte(u16 number);

u8 high_byte(u16 number);

u8 low_nibble(u8 number);

u8 high_nibble(u8 number);

bool carried_out_of(unsigned int bit_position, u16 a, u16 b, bool cf);

bool borrow_from(unsigned int bit_position, u16 a, u16 b, bool cf);
}
