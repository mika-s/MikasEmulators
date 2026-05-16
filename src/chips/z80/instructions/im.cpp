#include "chips/z80/interrupt_mode.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <iostream>

namespace emu::z80 {

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
void im(InterruptMode& interrupt_mode, const InterruptMode value, cyc& cycles)
{
    interrupt_mode = value;

    cycles = 8;
}

void print_im(std::ostream& ostream, const unsigned int interrupt_mode)
{
    ostream << "IM "
            << interrupt_mode;
}

TEST_CASE("Z80: IM")
{
    cyc cycles = 0;
    auto interrupt_mode = InterruptMode::ZERO;

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

    SUBCASE("should use 8 cycles")
    {
        cycles = 0;

        im(interrupt_mode, InterruptMode::ONE, cycles);

        CHECK_EQ(static_cast<cyc>(8), cycles);
    }
}
}
