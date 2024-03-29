#include "chips/z80/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>
#include <string>

namespace emu::z80 {

using emu::memory::NextByte;
using emu::util::byte::is_bit_set;
using emu::util::byte::set_bit;

void sra(u8& value, Flags& flag_reg)
{
    bool const should_set_carry_flag = is_bit_set(value, lsb);
    bool const is_previous_sign_bit_set = is_bit_set(value, msb);
    value = value >> 1;
    if (should_set_carry_flag) {
        flag_reg.set_carry_flag();
    } else {
        flag_reg.clear_carry_flag();
    }
    if (is_previous_sign_bit_set) {
        set_bit(value, msb);
    }

    flag_reg.clear_half_carry_flag();
    flag_reg.handle_zero_flag(value);
    flag_reg.handle_sign_flag(value);
    flag_reg.handle_parity_flag(value);
    flag_reg.clear_add_subtract_flag();
    flag_reg.handle_xy_flags(value);
}

/**
 * Shift right arithmetic (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to rotate, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sra_r(u8& reg, Flags& flag_reg, cyc& cycles)
{
    sra(reg, flag_reg);

    cycles = 8;
}

/**
 * Shift right arithmetic (the value in memory at HL's address)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 15</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param value_in_hl is the value in memory at HL's address, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sra_MHL(EmulatorMemory<u16, u8>& memory, u16 address, Flags& flag_reg, cyc& cycles)
{
    u8 value = memory.read(address);

    sra(value, flag_reg);

    memory.write(address, value);

    cycles = 15;
}

/**
 * Shift right arithmetic (value in memory pointed to by IX or IY plus d)
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 23</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param ixy_reg is the IX or IY register containing the base address
 * @param d is the address offset
 * @param memory is the memory, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sra_MixyPd(u16 ixy_reg, u8 d, EmulatorMemory<u16, u8>& memory, Flags& flag_reg, cyc& cycles)
{
    const u16 address = ixy_reg + static_cast<i8>(d);
    u8 value = memory.read(address);

    sra(value, flag_reg);

    memory.write(address, value);

    cycles = 23;
}

/**
 * Shift right arithmetic (value in memory pointed to by IX or IY plus d)
 * <ul>
 *   <li>Size: 4</li>
 *   <li>Cycles: 6</li>
 *   <li>States: 23</li>
 *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
 * </ul>
 *
 * @param reg is the register store the result in, which will be mutated
 * @param ixy_reg is the IX or IY register containing the base address
 * @param d is the address offset
 * @param memory is the memory, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sra_MixyPd_r(u8& reg, u16 ixy_reg, u8 d, EmulatorMemory<u16, u8>& memory, Flags& flag_reg, cyc& cycles)
{
    const u16 address = ixy_reg + static_cast<i8>(d);
    u8 value = memory.read(address);

    sra(value, flag_reg);

    memory.write(address, value);
    reg = value;

    cycles = 23;
}

void print_sra(std::ostream& ostream, std::string const& reg)
{
    ostream << "SRA "
            << reg;
}

void print_sra_MixyPn(std::ostream& ostream, unsigned int bit_number, std::string const& ixy_reg, u8 d)
{
    const i8 signed_value = static_cast<i8>(d);
    const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

    ostream << "SRA "
            << bit_number
            << ", ("
            << ixy_reg
            << plus_or_minus
            << +signed_value
            << ")";
}
}
