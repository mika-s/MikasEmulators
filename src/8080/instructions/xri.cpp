#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080 {

    using emu::util::string::hexify_wo_0x;

    /**
     * Exclusive or immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xri(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        acc_reg ^= args.farg;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by XRI, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but should be reset according to the 8080/8085 Assembly Language Programming
        // Manual. It is reset in this emulator.

        flag_reg.clear_aux_carry_flag();

        cycles = 7;
    }

    void print_xri(std::ostream &ostream, const NextByte &args) {
        ostream << "XRI "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: XRI") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should xor the given value with the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    xri(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg_counter ^ value, acc_reg);
                }
            }
        }

        SUBCASE("should always clear the carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    xri(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(false, flag_reg.is_carry_flag_set());
                }
            }
        }

        SUBCASE("should set the zero flag when zero and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    xri(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not set it otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    xri(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;
            NextByte args = {0xFF};

            xri(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x2;
            NextByte args = {0xff};

            xri(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should always unset the aux carry flag") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    NextByte args = {value};
                    acc_reg = acc_reg_counter;

                    xri(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
                }
            }
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            acc_reg = 0xE;
            NextByte args = {0};
            Flags flag_reg;

            xri(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
