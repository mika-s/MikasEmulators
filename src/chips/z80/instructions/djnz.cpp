#include "crosscutting/memory/next_byte.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>

namespace emu::z80 {

using emu::memory::NextByte;
using emu::util::string::hexify_wo_0x;

/**
 * Decrement register and jump if not zero
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2 or 3</li>
 *   <li>States: 8 or 13</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param b_reg is the B register to decrement, which will be mutated
 * @param pc is the program counter, which will be mutated
 * @param args contains the argument with the displacement
 * @param cycles is the number of cycles variable, which will be mutated
 */
void djnz(u8& b_reg, u16& pc, NextByte const& args, cyc& cycles)
{
    cycles = 8;

    b_reg--;

    if (b_reg != 0) {
        pc += static_cast<i8>(args.farg);
        cycles += 5;
    }
}

void print_djnz(std::ostream& ostream, NextByte const& args)
{
    ostream << "DJNZ "
            << hexify_wo_0x(static_cast<i8>(args.farg));
}

TEST_CASE("Z80: DJNZ")
{
    cyc cycles = 0;

    SUBCASE("should decrement register and not do anything else if it's zero")
    {
        u16 pc = 0x100f;
        u8 b_reg = 0x01;

        djnz(b_reg, pc, { .farg = 0x03 }, cycles);

        CHECK_EQ(0x00, b_reg);
        CHECK_EQ(0x100f, pc);
    }

    SUBCASE("should decrement register and add a displacement if the register does not become 0")
    {
        u16 pc = 0x1000;
        u8 b_reg = 0x02;

        djnz(b_reg, pc, { .farg = 0x03 }, cycles);

        CHECK_EQ(0x01, b_reg);
        CHECK_EQ(0x1003, pc);
    }

    SUBCASE("should use 8 cycles when the register becomes 0")
    {
        u16 pc = 0x100f;
        u8 b_reg = 0x01;

        djnz(b_reg, pc, { .farg = 0x03 }, cycles);

        CHECK_EQ(8, cycles);
    }

    SUBCASE("should use 13 cycles when the register does not become 0")
    {
        u16 pc = 0x100f;
        u8 b_reg = 0x02;

        djnz(b_reg, pc, { .farg = 0x03 }, cycles);

        CHECK_EQ(13, cycles);
    }
}
}
