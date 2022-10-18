#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "chips/8080/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::i8080 {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;

    /**
     * Rotate left through carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ral(u8 &acc_reg, Flags &flag_reg, cyc &cycles) {
        const bool should_set_carry_flag = is_bit_set(acc_reg, msb);
        acc_reg = acc_reg << 1;
        if (flag_reg.is_carry_flag_set()) {
            set_bit(acc_reg, lsb);
        }
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        cycles = 4;
    }

    void print_ral(std::ostream &ostream) {
        ostream << "RAL";
    }

    TEST_CASE("8080: RAL") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should rotate the accumulator left") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;

                ral(acc_reg, flag_reg, cycles);

                CHECK_EQ(static_cast<u8>(acc_reg_counter << 1u), acc_reg);
            }
        }

        SUBCASE("should set the carry flag if shifted out of msb") {
            acc_reg = 0b10000000;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            ral(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should take the carry flag into account when shifting") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            ral(acc_reg, flag_reg, cycles);

            CHECK_EQ(0b00000001, acc_reg);
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            Flags flag_reg;

            ral(acc_reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
