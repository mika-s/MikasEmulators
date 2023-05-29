#include "chips/lr35902/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>
#include <string>

namespace emu::lr35902 {

using emu::memory::NextByte;
using emu::util::byte::is_bit_set;

void sla(u8& value, Flags& flag_reg)
{
    bool const should_set_carry_flag = is_bit_set(value, msb);
    value = value << 1;
    if (should_set_carry_flag) {
        flag_reg.set_carry_flag();
    } else {
        flag_reg.clear_carry_flag();
    }

    flag_reg.clear_half_carry_flag();
    flag_reg.handle_zero_flag(value);
    flag_reg.clear_add_subtract_flag();
}

/**
 * Shift left arithmetic (register)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param reg is the register to rotate, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sla_r(u8& reg, Flags& flag_reg, cyc& cycles)
{
    sla(reg, flag_reg);

    cycles = 8;
}

/**
 * Shift left arithmetic (the value in memory at HL's address)
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 4</li>
 *   <li>States: 16</li>
 *   <li>Condition bits affected: carry, half carry, zero, add/subtract</li>
 * </ul>
 *
 * @param value_in_hl is the value in memory at HL's address, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sla_MHL(EmulatorMemory<u16, u8>& memory, u16 address, Flags& flag_reg, cyc& cycles)
{
    u8 value = memory.read(address);

    sla(value, flag_reg);

    memory.write(address, value);

    cycles = 16;
}

void print_sla(std::ostream& ostream, std::string const& reg)
{
    ostream << "SLA "
            << reg;
}
}
