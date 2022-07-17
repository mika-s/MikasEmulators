#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

    void cp(u8 &acc_reg, u8 value, Flags &flag_reg) {
        const u8 previous = acc_reg;
        const u8 new_acc_reg = previous - value;

        flag_reg.handle_borrow_flag(previous, value, false);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_half_borrow_flag(previous, value, false);
        flag_reg.set_add_subtract_flag();
    }

    /**
     * Compare register with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        cp(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Compare immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        cp(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Compare memory in HL's address with accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the value to compare with the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cp_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        cp(acc_reg, value, flag_reg);

        cycles = 7;
    }

    void print_cp(std::ostream &ostream, const std::string &reg) {
        ostream << "CP " << reg;
    }

    void print_cp(std::ostream &ostream, const NextByte &args) {
        ostream << "CP "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: CP") {
        u8 acc_reg = 0;
        Flags flag_reg;

        SUBCASE("should compare the accumulator with value and set flags") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    acc_reg = acc_reg_counter;

                    cp(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg - value) > 127, flag_reg.is_sign_flag_set());
                    CHECK_EQ(static_cast<u8>(acc_reg - value) == 0, flag_reg.is_zero_flag_set());
                    CHECK_EQ(acc_reg < value, flag_reg.is_carry_flag_set());
                }
            }
        }
    }

    TEST_CASE("Z80: CP r") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            cp_r(acc_reg, 0, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: CP n") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            NextByte args = {.farg = 0};
            Flags flag_reg;

            cp_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: CP [HL]") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            cp_MHL(acc_reg, 0, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
