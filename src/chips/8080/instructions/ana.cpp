#include "chips/8080/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include "instructions.h"
#include <cstdint>
#include <iostream>
#include <string>

namespace emu::i8080 {

    using emu::util::byte::is_bit_set;

    void ana(u8 &acc_reg, u8 value, Flags &flag_reg) {
        const u8 previous = acc_reg;
        acc_reg &= value;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by ANA, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but apparently has to be modified anyway. This is explained here:
        // https://demin.ws/blog/english/2012/12/24/my-i8080-collection/
        // The 8080/8085 Assembly Language Programming Manual also mentions auxiliary carry being modified by ANI.

        if (((previous | value) & 0x08) != 0) {
            flag_reg.set_aux_carry_flag();
        } else {
            flag_reg.clear_aux_carry_flag();
        }
    }

    /**
     * Logical and register with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param reg contains the argument that should be anded with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ana_r(u8 &acc_reg, u8 reg, Flags &flag_reg, cyc &cycles) {
        ana(acc_reg, reg, flag_reg);

        cycles = 4;
    }

    /**
     * Logical and value in memory with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value_in_memory contains the argument that should be anded with the accumulator
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ana_m(u8 &acc_reg, u8 value_in_memory, Flags &flag_reg, cyc &cycles) {
        ana(acc_reg, value_in_memory, flag_reg);

        cycles = 7;
    }

    void print_ana(std::ostream &ostream, const std::string &reg) {
        ostream << "ANA "
                << reg;
    }

    TEST_CASE("8080: ANA") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should and given value with the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ana_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg_counter & value, acc_reg);
                }
            }
        }

        SUBCASE("should always clear the carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ana_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(false, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should set the zero flag when zero and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ana_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ana_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg > INT8_MAX, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;
            u8 value = 0xff;

            ana_r(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x2;
            u8 value = 0xff;

            ana_r(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set the aux carry when the bitwise ored third bit is set") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ana_r(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(is_bit_set(acc_reg_counter | value, 3), flag_reg.is_aux_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            acc_reg = 0xe;
            u8 value = 0;
            Flags flag_reg;

            ana_r(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            acc_reg = 0xe;
            EmulatorMemory<u16, u8> memory;
            memory.add({0x10});
            u16 address = 0x0000;
            Flags flag_reg;

            ana_m(acc_reg, memory.read(address), flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
