#pragma once

#include "crosscutting/typedefs.h"

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lr35902 {

using emu::memory::EmulatorMemory;

void set_bit_in_memory(EmulatorMemory<u16, u8>& memory, u16 address, unsigned int bit_position);

void unset_bit_in_memory(EmulatorMemory<u16, u8>& memory, u16 address, unsigned int bit_position);
}
