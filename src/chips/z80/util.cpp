#include "util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

using emu::util::byte::set_bit;
using emu::util::byte::unset_bit;

void set_bit_in_memory(EmulatorMemory<u16, u8>& memory, u16 address, unsigned int bit_position)
{
    u8 value = memory.read(address);
    set_bit(value, bit_position);
    memory.write(address, value);
}

void unset_bit_in_memory(EmulatorMemory<u16, u8>& memory, u16 address, unsigned int bit_position)
{
    u8 value = memory.read(address);
    unset_bit(value, bit_position);
    memory.write(address, value);
}
}
