#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::i8080 {

using emu::util::byte::to_u16;

/**
 * Move H&L to SP
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 3</li>
 *   <li>States: 5</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param sp is the stack pointer, which will be mutated
 * @param address_in_hl is the address stored in the HL registers
 * @param cycles is the number of cycles variable, which will be mutated
 */
void sphl(u16& sp, u16 address_in_hl, cyc& cycles)
{
    sp = address_in_hl;

    cycles = 5;
}

void print_sphl(std::ostream& ostream)
{
    ostream << "SPHL";
}

TEST_CASE("8080: SPHL")
{
    cyc cycles = 0;
    u16 sp = 0;
    u8 h_reg = 0x11;
    u8 l_reg = 0x22;

    SUBCASE("should move HL into SP")
    {
        sphl(sp, to_u16(h_reg, l_reg), cycles);

        CHECK_EQ(0x1122, sp);
    }

    SUBCASE("should use 5 cycles")
    {
        cycles = 0;

        sphl(sp, to_u16(h_reg, l_reg), cycles);

        CHECK_EQ(5, cycles);
    }
}
}
