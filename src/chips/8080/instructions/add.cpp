#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include "instruction_util.h"
#include "instructions.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::i8080 {

    void add(u8 &acc_reg, u8 value, Flags &flag_reg) {
        add_to_register(acc_reg, value, false, flag_reg);
    }

    /**
     * Add register to accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void add_r(u8 &acc_reg, u8 reg, Flags &flag_reg, cyc &cycles) {
        add(acc_reg, reg, flag_reg);

        cycles = 4;
    }

    /**
     * Add value in memory to accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value_in_memory is the value to add to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void add_m(u8 &acc_reg, u8 value_in_memory, Flags &flag_reg, cyc &cycles) {
        add(acc_reg, value_in_memory, flag_reg);

        cycles = 7;
    }

    void print_add(std::ostream &ostream, const std::string &reg) {
        ostream << "ADD "
                << reg;
    }

    TEST_CASE("8080: ADD") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should add given value to the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter + value), acc_reg);
                }
            }
        }

        SUBCASE("should add given value to the accumulator without taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0;

            add_r(acc_reg, 0xab, flag_reg, cycles);

            CHECK_EQ(0xab, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x0;

            add_r(acc_reg, 0x3, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x0;

            add_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            add_r(acc_reg, 0x80, flag_reg, cycles);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x1;

            add_r(acc_reg, 0x2, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the aux carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xf;

            add_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0x10, acc_reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should not set the aux carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            add_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(0xf, acc_reg);
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xe;

            add_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xe;
            EmulatorMemory memory;
            memory.add({0x10});
            u16 address = 0x0000;

            add_m(acc_reg, memory.read(address), flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
