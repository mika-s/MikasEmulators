#pragma once

#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include <cstddef>

namespace emu::util::byte {

using emu::misc::UInteger;

auto is_bit_set(u8 variable, unsigned int bit_position) -> bool;

template<std::size_t M>
auto is_bit_set(UInteger<M> variable, unsigned int bit_position) -> bool
{
    return (variable.underlying() >> bit_position) & 1;
}

void set_bit(u8& variable, unsigned int bit_position);

void unset_bit(u8& variable, unsigned int bit_position);

auto to_u16(u8 farg, u8 sarg) -> u16;

auto to_u32(u8 fiarg, u8 sarg, u8 targ, u8 foarg) -> u32;

auto low_byte(u16 number) -> u8;

auto high_byte(u16 number) -> u8;

auto low_nibble(u8 number) -> u8;

auto high_nibble(u8 number) -> u8;

auto carried_out_of(unsigned int bit_position, u16 a, u16 b, bool cf) -> bool;

auto borrow_from(unsigned int bit_position, u16 a, u16 b, bool cf) -> bool;
}
