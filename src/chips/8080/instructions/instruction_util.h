#pragma once

#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080 {
class Flags;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::i8080 {

using emu::memory::EmulatorMemory;
using emu::memory::NextWord;

static constexpr unsigned int lsb = 0;
static constexpr unsigned int msb = 7;

void add_to_register(u8& acc_reg, u8 value, bool cf, Flags& flag_reg);

void sub_from_register(u8& acc_reg, u8 value, bool cf, Flags& flag_reg);

void execute_call(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args);

void execute_call(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, u8 farg, u8 sarg);

void execute_return(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory);
}
