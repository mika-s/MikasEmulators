#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {

/**
 * Enable interrupts
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
void ei(bool& ime, cyc& cycles)
{
    ime = true;

    cycles = 4;
}

void print_ei(std::ostream& ostream)
{
    ostream << "EI";
}

TEST_CASE("LR35902: EI")
{
    cyc cycles = 0;

    SUBCASE("should enable interrupts")
    {
        bool ime = false;

        ei(ime, cycles);

        CHECK_EQ(true, ime);

        ei(ime, cycles);

        CHECK_EQ(true, ime);
    }

    SUBCASE("should use 4 cycles")
    {
        cycles = 0;
        bool ime = false;

        ei(ime, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
