#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

    /**
     * Subtract immediate with borrow
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbi(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        sub_from_register(acc_reg, args.farg, flag_reg.is_carry_flag_set(), flag_reg);

        cycles = 7;
    }

    void print_sbi(std::ostream &ostream, const NextByte &args) {
        ostream << "SBI "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: SBI") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;
                    NextByte args = {value};

                    sbi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter - value), acc_reg);
                }
            }
        }

        SUBCASE("should subtract the given value to the accumulator with taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0xac;
            NextByte args = {0xab};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;
                    NextByte args = {value};

                    sbi(acc_reg, args, flag_reg, cycles);

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

                    sbi(acc_reg, args, flag_reg, cycles);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the parity flag when even parity") {
            Flags flag_reg;
            acc_reg = 0x3;
            NextByte args = {0};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(true, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should not set the parity flag when odd parity") {
            Flags flag_reg;
            acc_reg = 0x1;
            NextByte args = {0};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_parity_flag_set());
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x5;
            NextByte args = {0x6};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xff, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x3;
            NextByte args = {0x2};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the aux carry flag") {
            Flags flag_reg;
            acc_reg = 0xf;
            NextByte args = {0x1};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xe, acc_reg);
            CHECK_EQ(true, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should not set the aux carry flag") {
            Flags flag_reg;
            acc_reg = 0x10;
            NextByte args = {0x1};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(0xf, acc_reg);
            CHECK_EQ(false, flag_reg.is_aux_carry_flag_set());
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;
            Flags flag_reg;
            acc_reg = 0xe;
            NextByte args = {0x1};

            sbi(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
