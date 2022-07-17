#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instructions.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::string::hexify_wo_0x;

    void or_(u8 &acc_reg, u8 value, Flags &flag_reg) {
        acc_reg |= value;

        flag_reg.clear_carry_flag();
        flag_reg.clear_half_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);
        flag_reg.clear_add_subtract_flag();
    }

    /**
     * Inclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * According to the Z80 CPU User Manual, the parity/overflow flag should be set when overflowing for
     * this instruction. That is not correct, it should be set when even parity.
     * See https://retrocomputing.stackexchange.com/questions/2549/z80-cpu-user-manual-and-p-v-flag.
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void or_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        or_(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Or immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * According to the Z80 CPU User Manual, the parity/overflow flag should be set when overflowing for
     * this instruction. That is not correct, it should be set when even parity.
     * See https://retrocomputing.stackexchange.com/questions/2549/z80-cpu-user-manual-and-p-v-flag.
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void or_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        or_(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Inclusive or memory in HL's address with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * According to the Z80 CPU User Manual, the parity/overflow flag should be set when overflowing for
     * this instruction. That is not correct, it should be set when even parity.
     * See https://retrocomputing.stackexchange.com/questions/2549/z80-cpu-user-manual-and-p-v-flag.
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void or_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        or_(acc_reg, value, flag_reg);

        cycles = 7;
    }

    void print_or_r(std::ostream &ostream, const std::string &reg) {
        ostream << "OR "
                << reg;
    }

    void print_or_n(std::ostream &ostream, const NextByte &args) {
        ostream << "OR "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: OR") {
        u8 acc_reg = 0;

        SUBCASE("should or the given value with the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg_counter | value, acc_reg);
                }
            }
        }

        SUBCASE("should always reset the carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(false, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should set the zero flag when zero and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity/overflow flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;
            u8 value = 0xff;

            or_(acc_reg, value, flag_reg);

            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should not set the parity/overflow flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x2;
            u8 value = 0xfe;

            or_(acc_reg, value, flag_reg);

            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should always reset the half carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
                }
            }
        }

        SUBCASE("should always reset the add/subtract flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    or_(acc_reg, value, flag_reg);

                    CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
                }
            }
        }
    }

    TEST_CASE("Z80: OR r") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            u8 acc_reg = 0xe;
            u8 value = 0;
            Flags flag_reg;

            or_r(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: OR n") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            u8 acc_reg = 0xe;
            NextByte args = {0};
            Flags flag_reg;

            or_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: OR [HL]") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            u8 acc_reg = 0xe;
            u8 value = 0;
            Flags flag_reg;

            or_MHL(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
