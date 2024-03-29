#include "chips/lr35902/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::lr35902 {

using emu::util::byte::is_bit_set;

/**
 * Set carry flag
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: carry, half carry, add/subtract</li>
 * </ul>
 *
 * @param flag_reg is the flag register, which will be mutated
 * @param acc_reg is the accumulator, and is used to set the X and Y flags
 * @param cycles is the number of cycles variable, which will be mutated
 */
void scf(Flags& flag_reg, cyc& cycles)
{
    flag_reg.set_carry_flag();
    flag_reg.clear_half_carry_flag();
    flag_reg.clear_add_subtract_flag();

    cycles = 4;
}

void print_scf(std::ostream& ostream)
{
    ostream << "SCF";
}

TEST_CASE("LR35902: SCF")
{
    cyc cycles = 0;

    SUBCASE("should set the carry flag and always reset the half carry and add/subtract flags")
    {
        Flags flag_reg;

        CHECK_EQ(false, flag_reg.is_carry_flag_set());

        scf(flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_zero_flag_set());

        flag_reg.set_half_carry_flag();
        flag_reg.set_add_subtract_flag();
        scf(flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_zero_flag_set());

        flag_reg.set_half_carry_flag();
        flag_reg.set_add_subtract_flag();
        scf(flag_reg, cycles);

        CHECK_EQ(true, flag_reg.is_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        CHECK_EQ(false, flag_reg.is_zero_flag_set());
    }

    SUBCASE("should use 4 cycles")
    {
        Flags flag_reg;
        cycles = 0;

        scf(flag_reg, cycles);

        CHECK_EQ(4, cycles);
    }
}
}
