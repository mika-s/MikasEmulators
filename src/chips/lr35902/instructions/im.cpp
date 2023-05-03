#include "chips/lr35902/interrupt_mode.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {

/**
 * Set interrupt mode
 * <ul>
 *   <li>Size: 2</li>
 *   <li>Cycles: 2</li>
 *   <li>States: 8</li>
 *   <li>Condition bits affected: none</li>
 * </ul>
 *
 * @param interrupt_mode is the interrupt mode, which will be mutated
 * @param value is the value to set the interrupt mode to
 * @param cycles is the number of cycles variable, which will be mutated
 */
void im(InterruptMode& interrupt_mode, InterruptMode value, cyc& cycles)
{
    interrupt_mode = value;

    cycles = 8;
}

void print_im(std::ostream& ostream, unsigned int interrupt_mode)
{
    ostream << "IM "
            << interrupt_mode;
}

TEST_CASE("LR35902: IM")
{
    cyc cycles = 0;
    InterruptMode interrupt_mode = InterruptMode::ZERO;

    SUBCASE("should set a new interrupt mode")
    {
        cycles = 0;

        im(interrupt_mode, InterruptMode::ONE, cycles);

        CHECK_EQ(InterruptMode::ONE, interrupt_mode);

        im(interrupt_mode, InterruptMode::TWO, cycles);

        CHECK_EQ(InterruptMode::TWO, interrupt_mode);

        im(interrupt_mode, InterruptMode::ZERO, cycles);

        CHECK_EQ(InterruptMode::ZERO, interrupt_mode);
    }

    SUBCASE("should use 10 cycles")
    {
        cycles = 0;

        im(interrupt_mode, InterruptMode::ONE, cycles);

        CHECK_EQ(8, cycles);
    }
}
}
