#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    void sbc(u8 &acc_reg, u8 value, Flags &flag_reg) {
        sub_from_register(acc_reg, value, flag_reg.is_carry_flag_set(), flag_reg);
    }

    /**
     * Subtract with carry
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
    void sbc_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        sbc(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Subtract immediate with carry
     * <ul>
     *   <li>Size: 1</li>
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
    void sbc_A_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        sbc(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Subtract memory in HL's address to accumulator with carry
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
    void sbc_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        sbc(acc_reg, value, flag_reg);

        cycles = 7;
    }

    /**
     * Subtract register pair from HL with carry
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbc_HL_ss(u8 &h_reg, u8 &l_reg, u8 reg1, u8 &reg2, Flags &flag_reg, unsigned long &cycles) {
        u16 hl = to_u16(h_reg, l_reg);
        u16 value = to_u16(reg1, reg2);
        sub_from_register(hl, value, flag_reg.is_carry_flag_set(), flag_reg);
        h_reg = second_byte(hl);
        l_reg = first_byte(hl);

        flag_reg.set_add_subtract_flag();

        cycles = 15;
    }

    void print_sbc(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "SBC "
                << dest
                << ", "
                << src;
    }

    void print_sbc(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "SBC "
                << reg
                << ", "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: SBC") {
        u8 acc_reg = 0;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sbc(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter - value), acc_reg);
                }
            }
        }

        SUBCASE("should subtract the given value from the accumulator taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0xab;

            sbc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xa9, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sbc(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    sbc(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

//        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
//            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
//                for (u8 value = 0; value < UINT8_MAX; ++value) {
//                    Flags flag_reg;
//                    acc_reg = acc_reg_counter;
//
//                    sbc(acc_reg, value, flag_reg);
//
//                    const bool are_same_sign = (acc_reg_counter <= 127 && value <= 127)
//                                               || (127 < acc_reg_counter && 127 < value);
//                    const bool are_positive = are_same_sign && acc_reg_counter <= 127;
//                    const bool are_negative = are_same_sign && 127 < acc_reg_counter;
//                    const bool goes_from_positive_to_negative = are_positive && acc_reg > 127;
//                    const bool goes_negative_to_positive = are_negative && acc_reg <= 127;
//                    if (goes_from_positive_to_negative || goes_negative_to_positive != flag_reg.is_parity_overflow_flag_set()) {
//                        std::cout << "";
//                    }
//                    CHECK_EQ(
//                            goes_from_positive_to_negative || goes_negative_to_positive,
//                            flag_reg.is_parity_overflow_flag_set()
//                    );
//                }
//            }
//        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            sbc(acc_reg, 0x81, flag_reg);

            CHECK_EQ(0xff, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x2;

            sbc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0x1, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the half carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            sbc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xd, acc_reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should not set the half carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0x10;

            sbc(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xF, acc_reg);
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }
    }

    TEST_CASE("Z80: SBC A, n") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;

            sbc_A_r(acc_reg, 0x1, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: SBC A, n") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;
            NextByte args = {0x1};

            sbc_A_n(acc_reg, args, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: SBC A, (HL)") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;
            NextByte args = {0x1};

            sbc_A_MHL(acc_reg, args.farg, flag_reg, cycles);

            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: SBC HL, ss)") {
        unsigned long cycles = 0;
        u8 h_reg, l_reg;
        u8 reg1, reg2;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u16 hl_counter = 0; hl_counter < UINT8_MAX; ++hl_counter) {   // Too many cases with UINT16_MAX
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    reg1 = second_byte(value);
                    reg2 = first_byte(value);

                    sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

                    CHECK_EQ(static_cast<u16>(hl_counter - value), to_u16(h_reg, l_reg));
                }
            }
        }

        SUBCASE("should subtract the given value from the HL register pair taking carry into account") {
            const u16 hl = 0xab;
            h_reg = second_byte(hl);
            l_reg = first_byte(hl);

            const u16 bc = 0x1;
            reg1 = second_byte(bc);
            reg2 = first_byte(bc);

            Flags flag_reg;
            flag_reg.set_carry_flag();

            sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

            CHECK_EQ(0xa9, to_u16(h_reg, l_reg));
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u16 hl_counter = 0; hl_counter < UINT8_MAX; ++hl_counter) {   // Too many cases with UINT16_MAX
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    reg1 = second_byte(value);
                    reg2 = first_byte(value);

                    sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

                    CHECK_EQ(to_u16(h_reg, l_reg) == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 32767 and not otherwise") {
            const u8 min_max = 100;
            for (u16 hl_counter = UINT16_MAX / 2 - min_max; hl_counter < static_cast<u16>(UINT16_MAX / 2 + min_max); ++hl_counter) {
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    reg1 = second_byte(value);
                    reg2 = first_byte(value);

                    sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

                    CHECK_EQ(to_u16(h_reg, l_reg) > UINT16_MAX / 2, flag_reg.is_sign_flag_set());
                }
            }
        }

//        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
//            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
//                for (u8 value = 0; value < UINT8_MAX; ++value) {
//                    Flags flag_reg;
//                    acc_reg = acc_reg_counter;
//
//                    sbc(acc_reg, value, flag_reg);
//
//                    const bool are_same_sign = (acc_reg_counter <= 127 && value <= 127)
//                                               || (127 < acc_reg_counter && 127 < value);
//                    const bool are_positive = are_same_sign && acc_reg_counter <= 127;
//                    const bool are_negative = are_same_sign && 127 < acc_reg_counter;
//                    const bool goes_from_positive_to_negative = are_positive && acc_reg > 127;
//                    const bool goes_negative_to_positive = are_negative && acc_reg <= 127;
//                    if (goes_from_positive_to_negative || goes_negative_to_positive != flag_reg.is_parity_overflow_flag_set()) {
//                        std::cout << "";
//                    }
//                    CHECK_EQ(
//                            goes_from_positive_to_negative || goes_negative_to_positive,
//                            flag_reg.is_parity_overflow_flag_set()
//                    );
//                }
//            }
//        }

        SUBCASE("should set the carry flag when borrowed out of msb") {
            const u16 hl = 0x8000;
            h_reg = second_byte(hl);
            l_reg = first_byte(hl);

            const u16 bc = 0x8001;
            reg1 = second_byte(bc);
            reg2 = first_byte(bc);

            Flags flag_reg;

            sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

            CHECK_EQ(0xffff, to_u16(h_reg, l_reg));
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

//        SUBCASE("should not set the carry flag when not carried out of msb") {
//            Flags flag_reg;
//            acc_reg = 0x2;
//
//            sbc(acc_reg, 0x1, flag_reg);
//
//            CHECK_EQ(0x1, acc_reg);
//            CHECK_EQ(false, flag_reg.is_carry_flag_set());
//        }
//
//        SUBCASE("should set the half carry flag when carried out of the fourth bit") {
//            Flags flag_reg;
//            acc_reg = 0xe;
//
//            sbc(acc_reg, 0x1, flag_reg);
//
//            CHECK_EQ(0xd, acc_reg);
//            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
//        }
//
//        SUBCASE("should not set the half carry flag when not carried out of the fourth bit") {
//            Flags flag_reg;
//            acc_reg = 0x10;
//
//            sbc(acc_reg, 0x1, flag_reg);
//
//            CHECK_EQ(0xF, acc_reg);
//            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
//        }

        SUBCASE("should use 15 cycles") {
            cycles = 0;

            const u16 hl = 0xab;
            h_reg = second_byte(hl);
            l_reg = first_byte(hl);

            const u16 bc = 0x1;
            reg1 = second_byte(bc);
            reg2 = first_byte(bc);

            Flags flag_reg;

            sbc_HL_ss(h_reg, l_reg, reg1, reg2, flag_reg, cycles);

            CHECK_EQ(15, cycles);
        }
    }
}
