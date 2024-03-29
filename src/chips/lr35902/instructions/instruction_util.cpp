#include "instruction_util.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/util/byte_util.h"
#include "flags.h"

namespace emu::lr35902 {

using emu::memory::NextWord;
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;

void add_to_register(u8& reg, u8 value, bool cf, Flags& flag_reg)
{
    const u8 previous = reg;
    const u8 to_add = value + (cf ? 1 : 0);
    reg += to_add;

    flag_reg.handle_carry_flag(previous, value, cf);
    flag_reg.handle_zero_flag(reg);
    flag_reg.handle_half_carry_flag(previous, value, cf);
    flag_reg.clear_add_subtract_flag();
}

void add_to_register(u16& reg, u16 value, bool cf, Flags& flag_reg)
{
    u8 reg_lo = low_byte(reg);
    u8 reg_hi = high_byte(reg);
    u8 value_lo = low_byte(value);
    u8 value_hi = high_byte(value);

    add_to_register(reg_lo, value_lo, cf, flag_reg);
    add_to_register(reg_hi, value_hi, flag_reg.is_carry_flag_set(), flag_reg);

    reg = to_u16(reg_hi, reg_lo);
    flag_reg.handle_zero_flag(reg);
}

void sub_from_register(u8& reg, u8 value, bool cf, Flags& flag_reg)
{
    add_to_register(reg, ~value, !cf, flag_reg);
    flag_reg.toggle_carry_flag();
    flag_reg.toggle_half_carry_flag();
    flag_reg.set_add_subtract_flag();
}

void sub_from_register(u16& reg, u16 value, bool cf, Flags& flag_reg)
{
    u8 reg_lo = low_byte(reg);
    u8 reg_hi = high_byte(reg);
    u8 value_lo = low_byte(value);
    u8 value_hi = high_byte(value);

    sub_from_register(reg_lo, value_lo, cf, flag_reg);
    sub_from_register(reg_hi, value_hi, flag_reg.is_carry_flag_set(), flag_reg);

    reg = to_u16(reg_hi, reg_lo);
    flag_reg.handle_zero_flag(reg);
}

void execute_call(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, NextWord const& args)
{
    execute_call(pc, sp, memory, args.farg, args.sarg);
}

void execute_call(u16& pc, u16& sp, EmulatorMemory<u16, u8>& memory, u8 farg, u8 sarg)
{
    memory.write(--sp, high_byte(pc));
    memory.write(--sp, low_byte(pc));

    pc = to_u16(sarg, farg);
}

void execute_return(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory)
{
    const u8 sarg = memory.read(sp++);
    const u8 farg = memory.read(sp++);
    pc = to_u16(farg, sarg);
}
}
