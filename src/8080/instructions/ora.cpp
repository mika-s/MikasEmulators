#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080 {
    /**
     * Inclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ora(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        ora(acc_reg, value, flag_reg, cycles, false);
    }

    /**
     * Inclusive or with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value contains the argument that should be ored with the accumulator
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void
    ora(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles, bool is_memory_involved) {
        acc_reg |= value;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by ORA, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but should be reset according to the 8080/8085 Assembly Language Programming
        // Manual. It is reset in this emulator.

        flag_reg.clear_aux_carry_flag();

        cycles = 4;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_ora(std::ostream &ostream, const std::string &reg) {
        ostream << "ORA "
                << reg;
    }

    TEST_CASE("8080: ORA") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should or the given value with the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ora(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg_counter | value, acc_reg);
                }
            }
        }

        SUBCASE("should always clear the carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ora(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(false, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should set the zero flag when zero and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ora(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ora(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;
            u8 value = 0xff;

            ora(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x2;
            u8 value = 0xfe;

            ora(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should always unset the aux carry") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    ora(acc_reg, value, flag_reg, cycles);

                    CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 4 cycles if memory is not involved") {
            cycles = 0;
            acc_reg = 0xe;
            u8 value = 0;
            Flags flag_reg;

            ora(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            acc_reg = 0xe;
            u8 value = 0;
            Flags flag_reg;

            ora(acc_reg, value, flag_reg, cycles, true);

            CHECK_EQ(7, cycles);
        }
    }
}
