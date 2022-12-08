#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instruction_util.h"
#include <iostream>
#include <vector>

namespace emu::i8080 {

using emu::util::byte::to_u16;

/**
 * Return if zero
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1 or 3</li>
 *   <li>States: 5 or 11</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param pc is the program counter, which will be mutated
 * @param sp is the stack pointer, which will be mutated
 * @param memory is the memory, which will be mutated
 * @param flag_reg is the flag register
 * @param cycles is the number of cycles variable, which will be mutated
 */
void rz(u16& pc, u16& sp, EmulatorMemory<u16, u8> const& memory, Flags const& flag_reg, cyc& cycles)
{
    cycles = 0;

    if (flag_reg.is_zero_flag_set()) {
        execute_return(pc, sp, memory);

        cycles += 6;
    }

    cycles += 5;
}

void print_rz(std::ostream& ostream)
{
    ostream << "RZ";
}

TEST_CASE("8080: RZ")
{
    cyc cycles = 0;
    EmulatorMemory<u16, u8> memory;
    memory.add(std::vector<u8> { 0xab, 0x01, 0x02, 0x03, 0x04, 0x05 });

    SUBCASE("should pop PC off the stack when the zero flag is set")
    {
        u16 pc = 0x100f;
        u16 sp = 0;

        Flags flag_reg;
        flag_reg.set_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);

        CHECK_EQ(to_u16(memory.read(1), memory.read(0)), pc);
        CHECK_EQ(0x2, sp);
    }

    SUBCASE("should not pop PC off the stack when the zero flag is unset")
    {
        u16 pc = 0x100f;
        u16 sp = 0;
        Flags flag_reg;
        flag_reg.clear_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);

        CHECK_EQ(0x100f, pc);
        CHECK_EQ(0, sp);
    }

    SUBCASE("should use 5 cycles when not returning")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0;
        Flags flag_reg;

        rz(pc, sp, memory, flag_reg, cycles);

        CHECK_EQ(5, cycles);
    }

    SUBCASE("should use 11 cycles when returning")
    {
        cycles = 0;
        u16 pc = 0;
        u16 sp = 0;
        Flags flag_reg;
        flag_reg.set_zero_flag();

        rz(pc, sp, memory, flag_reg, cycles);

        CHECK_EQ(11, cycles);
    }
}
}
