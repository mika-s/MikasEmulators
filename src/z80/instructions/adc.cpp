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

    void adc(u8 &acc_reg, u8 value, Flags &flag_reg) {
        add_to_register(acc_reg, value, flag_reg.is_carry_flag_set(), flag_reg);
        flag_reg.clear_add_subtract_flag();
    }

    /**
     * Add from register to accumulator with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        adc(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Add immediate to accumulator with carry
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
    void adc_A_n(u8 &acc_reg, NextByte args, Flags &flag_reg, unsigned long &cycles) {
        adc(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Add memory in HL's address to accumulator with carry
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adc_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        adc(acc_reg, value, flag_reg);

        cycles = 7;
    }

    void print_adc(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "ADC "
                << dest
                << ", "
                << src;
    }

    void print_adc(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "ADC "
                << reg
                << ", "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: ADC") {
        u8 acc_reg = 0;

        SUBCASE("should add given value to the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    adc(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter + value), acc_reg);
                }
            }
        }

        SUBCASE("should add given value to the accumulator and take carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0;

            adc(acc_reg, 0xab, flag_reg);

            CHECK_EQ(0xac, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    adc(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    adc(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    adc(acc_reg, value, flag_reg);

                    const bool are_same_sign = (acc_reg_counter <= 127 && value <= 127)
                                               || (127 < acc_reg_counter && 127 < value);
                    const bool are_positive = are_same_sign && acc_reg_counter <= 127;
                    const bool are_negative = are_same_sign && 127 < acc_reg_counter;
                    const bool goes_from_positive_to_negative = are_positive && acc_reg > 127;
                    const bool goes_negative_to_positive = are_negative && acc_reg <= 127;
                    CHECK_EQ(
                            goes_from_positive_to_negative || goes_negative_to_positive,
                            flag_reg.is_parity_overflow_flag_set()
                    );
                }
            }
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            adc(acc_reg, 0x80, flag_reg);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x1;

            adc(acc_reg, 0x2, flag_reg);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the half carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xf;

            adc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0x10, acc_reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should not set the half carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            adc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xf, acc_reg);
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }
    }

    TEST_CASE("Z80: ADC A, r") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            Flags flag_reg;

            adc_A_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: ADC A, n") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;
            NextByte args = {0x1};

            adc_A_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: ADC A, [HL]") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;

            adc_A_MHL(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
