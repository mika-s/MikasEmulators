#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {

/**
 * Disable interrupts
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param ime is the interrupt master enable, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void di(bool& ime, cyc& cycles)
{
    ime = false;

    cycles = 4;
}

void print_di(std::ostream& ostream)
{
    ostream << "DI";
}

TEST_CASE("LR35902: DI")
{
    cyc cycles = 0;

    SUBCASE("should disable interrupts")
    {
        bool ime = true;

        di(ime, cycles);

        CHECK_EQ(false, ime);

        di(ime, cycles);

        CHECK_EQ(false, ime);
    }

    SUBCASE("should use 4 cycles")
    {
        cycles = 0;
        bool ime = true;

        di(ime, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
