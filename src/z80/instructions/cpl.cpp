#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {
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
    void cpl(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        acc_reg = ~acc_reg;

        flag_reg.set_half_carry_flag();
        flag_reg.set_add_subtract_flag();

        cycles = 4;
    }

    void print_cpl(std::ostream &ostream) {
        ostream << "CPL";
    }

    TEST_CASE("Z80: CPL") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;


        SUBCASE("should complement the accumulator") {
            Flags flag_reg;

            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cpl(acc_reg, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(~acc_reg_counter), acc_reg);
                }
            }
        }

        SUBCASE("should always set the half carry flag") {
            Flags flag_reg;
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always set the add/subtract flag") {
            Flags flag_reg;
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            Flags flag_reg;

            cpl(acc_reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
