#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

    void sub(u8 &acc_reg, u8 value, Flags &flag_reg) {
        sub_from_register(acc_reg, value, false, flag_reg);
        flag_reg.set_add_subtract_flag();
    }

    /**
     * Subtract
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        sub(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Subtract immediate from the accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        sub(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Subtract memory in HL's address from the accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        sub(acc_reg, value, flag_reg);

        cycles = 7;
    }

    void print_sub(std::ostream &ostream, const std::string &reg) {
        ostream << "SUB "
                << reg;
    }

    void print_sub(std::ostream &ostream, const NextByte &args) {
        ostream << "SUB "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: SUB") {
        u8 acc_reg = 0;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sub(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter - value), acc_reg);
                }
            }
        }

        SUBCASE("should subtract the given value from the accumulator without taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0xab;

            sub(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xaa, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sub(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sub(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

//        SUBCASE("should set the parity flag when even parity") {
//            Flags flag_reg;
//            acc_reg = 0x3;
//
//            sub(acc_reg, 0x0, flag_reg);
//
//            CHECK_EQ(0x3, acc_reg);
//            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
//        }
//
//        SUBCASE("should not set the parity flag when odd parity") {
//            Flags flag_reg;
//            acc_reg = 0x1;
//
//            sub(acc_reg, 0x0, flag_reg);
//
//            CHECK_EQ(0x1, acc_reg);
//            CHECK_EQ(false, flag_reg.is_parity_overflow_flag_set());
//        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            sub(acc_reg, 0x81, flag_reg);

            CHECK_EQ(0xff, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x2;

            sub(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the aux carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            sub(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xd, acc_reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should not set the aux carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0x10;

            sub(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xf, acc_reg);
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }
    }

    TEST_CASE("Z80: SUB r") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;

            sub_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: SUB n") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;
            NextByte args = {0x1};

            sub_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: SUB (HL)") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;
            NextByte args = {0x1};

            sub_MHL(acc_reg, args.farg, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
