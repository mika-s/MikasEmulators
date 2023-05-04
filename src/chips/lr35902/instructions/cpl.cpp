#include "chips/lr35902/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <cstdint>
#include <iostream>

namespace emu::lr35902 {

using emu::util::byte::is_bit_set;

/**
 * Complement accumulator
 * <ul>
 *   <li>Size: 1</li>
 *   <li>Cycles: 1</li>
 *   <li>States: 4</li>
 *   <li>Condition bits affected: half carry, add/subtract</li>
 * </ul>
 *
 * @param acc_reg is the accumulator register, which will be mutated
 * @param flag_reg is the flag register, which will be mutated
 * @param cycles is the number of cycles variable, which will be mutated
 */
void cpl(u8& acc_reg, Flags& flag_reg, cyc& cycles)
{
    acc_reg = ~acc_reg;

    flag_reg.set_half_carry_flag();
    flag_reg.set_add_subtract_flag();

    cycles = 4;
}

void print_cpl(std::ostream& ostream)
{
    ostream << "CPL";
}

TEST_CASE("LR35902: CPL")
{
    SUBCASE("should complement the accumulator and always set half carry and add/subtract flags")
    {
        cyc cycles = 0;
        u8 acc_reg = 0;
        Flags flag_reg;

        for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
            acc_reg = acc_reg_counter;

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(static_cast<u8>(~acc_reg_counter), acc_reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
            CHECK_EQ(4, cycles);
        }
    }
}
}
