#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::z80 {
/**
 * Halt
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param is_halted is the halted status variable, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void halt(bool& is_halted, cyc& cycles)
{
    is_halted = true;

    cycles = 4;
}

void print_halt(std::ostream& ostream)
{
    ostream << "HALT";
}

TEST_CASE("Z80: HALT")
{
    cyc cycles = 0;

    SUBCASE("should stop the program")
    {
        bool stopped = false;

        halt(stopped, cycles);

        CHECK_EQ(true, stopped);

        halt(stopped, cycles);

        CHECK_EQ(true, stopped);
    }

    SUBCASE("should use 4 cycles")
    {
        cycles = 0;

        bool stopped = false;

        halt(stopped, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
