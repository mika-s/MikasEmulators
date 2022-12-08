#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::i8080 {
/**
 * Halt
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 7</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param is_halted is the halted status variable, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void hlt(bool& is_halted, cyc& cycles)
{
    is_halted = true;

    cycles = 7;
}

void print_hlt(std::ostream& ostream)
{
    ostream << "HLT";
}

TEST_CASE("8080: HLT")
{
    cyc cycles = 0;

    SUBCASE("should stop the program")
    {
        bool is_halted = false;

        hlt(is_halted, cycles);

        CHECK_EQ(true, is_halted);

        hlt(is_halted, cycles);

        CHECK_EQ(true, is_halted);
    }

    SUBCASE("should use 7 cycles")
    {
        cycles = 0;

        bool stopped = false;

        hlt(stopped, cycles);

        CHECK_EQ(7, cycles);
    }
}
}
