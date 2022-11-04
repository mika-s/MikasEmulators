#include "chips/8080/flags.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::i8080 {

    void cmp(u8 &acc_reg, u8 arg, Flags &flag_reg) {
        const u8 previous = acc_reg;
        const u8 new_acc_reg = previous - arg;

        flag_reg.handle_borrow_flag(previous, arg, false);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_aux_borrow_flag(previous, arg, false);
    }

    /**
     * Compare register with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param reg is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cmp_r(u8 &acc_reg, u8 reg, Flags &flag_reg, cyc &cycles) {
        cmp(acc_reg, reg, flag_reg);

        cycles = 4;
    }

    /**
     * Compare value in memory with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value_in_memory is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cmp_m(u8 &acc_reg, u8 value_in_memory, Flags &flag_reg, cyc &cycles) {
        cmp(acc_reg, value_in_memory, flag_reg);

        cycles = 7;
    }

    void print_cmp(std::ostream &ostream, const std::string &reg) {
        ostream << "CMP " << reg;
    }

    TEST_CASE("8080: CMP") {
        cyc cycles = 0;
        u8 acc_reg = 0;
        Flags flag_reg;

        SUBCASE("should compare the accumulator with value and set flags") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cmp_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg - value) > INT8_MAX, flag_reg.is_sign_flag_set());
                    CHECK_EQ(static_cast<u8>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            u8 value = 0;

            cmp_r(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            u8 value = 0;

            cmp_m(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
