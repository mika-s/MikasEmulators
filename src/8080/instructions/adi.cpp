#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

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
    void adi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, cyc &cycles) {
        const u8 previous = acc_reg;
        acc_reg += args.farg;

        flag_reg.handle_aux_carry_flag(previous, args.farg, false);
        flag_reg.handle_carry_flag(previous, args.farg, false);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        cycles = 7;
    }

    void print_adi(std::ostream &ostream, const NextByte &args) {
        ostream << "ADI "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: ADI") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should add the given value to the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;
                    NextByte args = {value};

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter + value), acc_reg);
                }
            }
        }

        SUBCASE("should add the given value to the accumulator without taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0;
            NextByte args = {0xab};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xab, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    adi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x0;
            NextByte args = {0x3};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x0;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set the carry flag when carried out of MSB") {
            Flags flag_reg;
            acc_reg = 0x80;
            NextByte args = {0x80};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of MSB") {
            Flags flag_reg;
            acc_reg = 0x1;
            NextByte args = {0x2};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the aux carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xF;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x10, acc_reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should not set the aux carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xE;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xF, acc_reg);
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xE;
            NextByte args = {0x1};

            adi(acc_reg, args, flag_reg, cycles);
            CHECK_EQ(7, cycles);
        }
    }
}
