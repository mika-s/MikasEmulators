#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Add immediate
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void adi(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        const std::uint8_t previous = acc_reg;
        acc_reg += args.farg;

        flag_reg.handle_aux_carry_flag(previous, args.farg, false);
        flag_reg.handle_carry_flag(previous, args.farg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        cycles = 7;
    }

    void print_adi(std::ostream &ostream, const NextByte &args) {
        ostream << "ADI "
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: ADI") {
        unsigned long cycles = 0;
        std::uint8_t acc_reg = 0;

        SUBCASE("ShouldAddGivenValueToAccumulator") {
            for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;
                    NextByte args = {value};

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(static_cast<std::uint8_t>(acc_reg_counter + value), acc_reg);
                }
            }
        }

        SUBCASE("ShouldAddGivenValueToAccumulatorWithoutTakingCarryIntoAccount") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0;
            NextByte args = {0xab};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xab, acc_reg);
        }

        SUBCASE("ShouldSetZeroFlagWhenZeroAndNotSetOtherwise") {
            for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("ShouldSetSignFlagWhenAbove127AndNotOtherwise") {
            for (std::uint8_t acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (std::uint8_t value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("ShouldSetParityFlagWhenEvenParity") {
            Flags flag_reg;
            acc_reg = 0x0;
            NextByte args = {0x3};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("ShouldNotSetParityFlagWhenOddParity") {
            Flags flag_reg;
            acc_reg = 0x0;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("ShouldSetCarryFlagWhenCarriedOutOfMsb") {
            Flags flag_reg;
            acc_reg = 0x80;
            NextByte args = {0x80};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("ShouldNotSetCarryFlagWhenNotCarriedOutOfMsb") {
            Flags flag_reg;
            acc_reg = 0x1;
            NextByte args = {0x2};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("ShouldSetAuxCarryFlagWhenCarriedOutOfFourthBit") {
            Flags flag_reg;
            acc_reg = 0xF;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x10, acc_reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("ShouldNotSetAuxCarryFlagWhenNotCarriedOutOfFourthBit") {
            Flags flag_reg;
            acc_reg = 0xE;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xF, acc_reg);
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("ShouldUse7Cycles") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xE;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);
            CHECK_EQ(7, cycles);
        }
    }
}
