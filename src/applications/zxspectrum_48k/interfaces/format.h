#pragma once

#include "chips/z80/manual_state.h"
#include "crosscutting/memory/emulator_memory.h"

namespace emu::applications::zxspectrum_48k {

using emu::memory::EmulatorMemory;
using emu::z80::ManualState;

class Format {
public:
    virtual ~Format() = default;

    virtual void print_header() = 0;

    virtual ManualState to_cpu_state() = 0;

    virtual void to_memory(EmulatorMemory<u16, u8>& memory) = 0;
};
}
