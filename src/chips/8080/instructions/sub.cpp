#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "chips/8080/flags.h"
#include "instructions.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080 {
    /**
     * Subtract
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles) {
        sub(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Subtract
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value contains the value to subtract from the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sub(u8 &acc_reg, u8 value, Flags &flag_reg, cyc &cycles, bool is_memory_involved) {
        sub_from_register(acc_reg, value, false, flag_reg);

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_sub(std::ostream &ostream, const std::string &reg) {
        ostream << "SUB "
                << reg;
    }

    TEST_CASE("8080: SUB") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sub(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter - value), acc_reg);
                }
            }
        }

        SUBCASE("should subtract the given value from the accumulator without taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0xab;

            sub(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0xaa, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sub(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;
                    sub(acc_reg, value, flag_reg, cycles);
                    CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;

            sub(acc_reg, 0x0, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x1;

            sub(acc_reg, 0x0, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            sub(acc_reg, 0x81, flag_reg, cycles);

            CHECK_EQ(0xff, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x2;

            sub(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the aux carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            sub(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0xd, acc_reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should not set the aux carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0x10;

            sub(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0xf, acc_reg);
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xe;

            sub(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xe;

            sub(acc_reg, 0x1, flag_reg, cycles, true);

            CHECK_EQ(7, cycles);
        }
    }
}
