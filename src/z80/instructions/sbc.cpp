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
     * Add from IX or IY high or low to accumulator with carry
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbc_A_ixy_h_or_l(u8 &acc_reg, u8 ixy_reg_h_or_l, Flags &flag_reg, unsigned long &cycles) {
        sbc(acc_reg, ixy_reg_h_or_l, flag_reg);

        cycles = 8;
    }

    /**
     * Subtract value pointed to by IX or IY plus d from accumulator with carry
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 19</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbc_A_MixyPd(u8 &acc_reg, u16 ixy_reg, const NextByte &args, const EmulatorMemory &memory, Flags &flag_reg,
                      unsigned long &cycles
    ) {
        sbc(acc_reg, memory[ixy_reg + static_cast<i8>(args.farg)], flag_reg);

        cycles = 19;
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
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param value contains value to subtract from HL
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbc_HL_ss(u8 &h_reg, u8 &l_reg, u16 value, Flags &flag_reg, unsigned long &cycles) {
        u16 hl = to_u16(h_reg, l_reg);
        sub_from_register(hl, value, flag_reg.is_carry_flag_set(), flag_reg);
        h_reg = second_byte(hl);
        l_reg = first_byte(hl);

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

    void print_sbc_MixyPn(std::ostream &ostream, const std::string &reg, const std::string &ixy_reg,
                          const NextByte &args
    ) {
        const i8 signed_value = static_cast<i8>(args.farg);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "SBC "
                << reg
                << ",("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }

    TEST_CASE("Z80: SBC (byte)") {
        u8 acc_reg = 0;

        SUBCASE("should subtract the given value from the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    for (int carry = 0; carry < 2; ++carry) {
                        Flags flag_reg;
                        if (carry) {
                            flag_reg.set_carry_flag();
                        } else {
                            flag_reg.clear_carry_flag();
                        }

                        acc_reg = acc_reg_counter;

                        sbc(acc_reg, value, flag_reg);

                        CHECK_EQ(static_cast<u8>(acc_reg_counter - value - carry), acc_reg);
                        CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                        CHECK_EQ(static_cast<i8>(acc_reg) < 0, flag_reg.is_sign_flag_set());
                        CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
//                    CHECK_EQ(true, flag_reg.is_carry_flag_set());
                        CHECK_EQ(
                                (((acc_reg_counter & 0xf) - (value & 0xf) - (carry & 0xf)) & 0x10) > 0,
                                flag_reg.is_half_carry_flag_set()
                        );
                        // todo: overflow flag, carry flag
                    }

                }
            }
        }
    }

    TEST_CASE("Z80: SBC (word)") {
        unsigned long cycles = 0;
        u8 h_reg, l_reg;

        SUBCASE("should set the parity/overflow flag") {
            Flags flag_reg;
            h_reg = 0xfb;
            l_reg = 0x19;

            sbc_HL_ss(h_reg, l_reg, 31775, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u16 hl_counter = 0; hl_counter < UINT8_MAX; ++hl_counter) {   // Too many cases with UINT16_MAX
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    sbc_HL_ss(h_reg, l_reg, value, flag_reg, cycles);

                    CHECK_EQ(to_u16(h_reg, l_reg) == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 32767 and not otherwise") {
            const u8 min_max = 100;
            for (u16 hl_counter = UINT16_MAX / 2 - min_max;
                 hl_counter < static_cast<u16>(UINT16_MAX / 2 + min_max); ++hl_counter) {
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    sbc_HL_ss(h_reg, l_reg, value, flag_reg, cycles);

                    CHECK_EQ(to_u16(h_reg, l_reg) > UINT16_MAX / 2, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 32767 and not otherwise") {

            const u8 min_max = 100;
            for (u16 hl_counter = UINT16_MAX / 2 - min_max;
                 hl_counter < static_cast<u16>(UINT16_MAX / 2 + min_max); ++hl_counter) {
                for (u16 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    h_reg = second_byte(hl_counter);
                    l_reg = first_byte(hl_counter);

                    sbc_HL_ss(h_reg, l_reg, value, flag_reg, cycles);

                    CHECK_EQ(to_u16(h_reg, l_reg) > UINT16_MAX / 2, flag_reg.is_sign_flag_set());
                }
            }
        }
    }

    TEST_CASE("Z80: SBC A, r") {
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

    TEST_CASE("Z80: SBC HL, ss") {
        SUBCASE("should use 15 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 h_reg = 0xfb;
            u8 l_reg = 0x19;

            sbc_HL_ss(h_reg, l_reg, 31775, flag_reg, cycles);

            CHECK_EQ(15, cycles);
        }
    }
}
