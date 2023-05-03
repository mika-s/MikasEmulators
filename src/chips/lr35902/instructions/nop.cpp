#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {
/**
 * No operation
 * <ul>
 *   <li>Size: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param cycles is the number of cycles variable, which will be mutated
 */
void nop(cyc& cycles)
{
    cycles = 4;
}

void print_nop(std::ostream& ostream)
{
    ostream << "NOP";
}

TEST_CASE("LR35902: NOP")
{
    cyc cycles = 0;

    SUBCASE("should use 4 cycles")
    {
        nop(cycles);

        CHECK_EQ(4, cycles);
    }
}
}
