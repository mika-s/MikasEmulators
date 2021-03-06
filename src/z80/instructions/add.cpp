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

    /**
     * Add
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4 or 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void add(u8 &acc_reg, u8 value, Flags &flag_reg) {
        add_to_register(acc_reg, value, false, flag_reg);
        flag_reg.clear_add_subtract_flag();
    }

    /**
     * Add from register to accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void add_A_r(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        add(acc_reg, value, flag_reg);

        cycles = 4;
    }

    /**
     * Add immediate to accumulator
     * <ul>
     *   <li>Size: 2</li>
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
    void add_A_n(u8 &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        add(acc_reg, args.farg, flag_reg);

        cycles = 7;
    }

    /**
     * Add memory in HL's address to accumulator
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value to add_A_r to the accumulator register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void add_A_MHL(u8 &acc_reg, u8 value, Flags &flag_reg, unsigned long &cycles) {
        add(acc_reg, value, flag_reg);

        cycles = 7;
    }

    /**
     * Double register add
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param value_to_add contains the argument that should be added to HL
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void add_HL_ss(u8 &h_reg, u8 &l_reg, u16 value_to_add, Flags &flag_reg, unsigned long &cycles) {
        const u16 previous = to_u16(h_reg, l_reg);
        const u16 next = previous + value_to_add;
        h_reg = second_byte(next);
        l_reg = first_byte(next);

        flag_reg.handle_carry_flag(previous, value_to_add);
        flag_reg.handle_half_carry_flag(previous, value_to_add, false);
        flag_reg.clear_add_subtract_flag();

        cycles = 11;
    }

    /**
     * Double register add to IX or IY
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param ix_reg is the IX register, which will be mutated
     * @param value_to_add contains the argument that should be added to HL
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void add_ix_iy_pp(u16 &ix_reg, u16 value_to_add, Flags &flag_reg, unsigned long &cycles) {
        const u16 previous = ix_reg;
        ix_reg = previous + value_to_add;

        flag_reg.handle_carry_flag(previous, value_to_add);
        flag_reg.handle_half_carry_flag(previous, value_to_add, false);
        flag_reg.clear_add_subtract_flag();

        cycles = 15;
    }

    void print_add(std::ostream &ostream, const std::string &dest, const std::string &src) {
        ostream << "ADD "
                << dest
                << ", "
                << src;
    }

    void print_add_r_n(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "ADD "
                << reg
                << ", "
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: ADD") {
        u8 acc_reg = 0;

        SUBCASE("should add given value to the accumulator") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add(acc_reg, value, flag_reg);

                    CHECK_EQ(static_cast<u8>(acc_reg_counter + value), acc_reg);
                }
            }
        }

        SUBCASE("should add given value to the accumulator without taking carry into account") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0;

            add(acc_reg, 0xab, flag_reg);

            CHECK_EQ(0xab, acc_reg);
        }

        SUBCASE("should set the zero flag when zero and not set otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg == 0, flag_reg.is_zero_flag_set());
                }
            }
        }

        SUBCASE("should set the sign flag when above 127 and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add(acc_reg, value, flag_reg);

                    CHECK_EQ(acc_reg > 127, flag_reg.is_sign_flag_set());
                }
            }
        }

        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                for (u8 value = 0; value < UINT8_MAX; ++value) {
                    Flags flag_reg;
                    acc_reg = acc_reg_counter;

                    add(acc_reg, value, flag_reg);

                    const bool are_same_sign = (acc_reg_counter <= 127 && value <= 127)
                                               || (127 < acc_reg_counter && 127 < value);
                    const bool are_positive = are_same_sign && acc_reg_counter <= 127;
                    const bool are_negative = are_same_sign && 127 < acc_reg_counter;
                    const bool goes_from_positive_to_negative = are_positive && acc_reg > 127;
                    const bool goes_negative_to_positive = are_negative && acc_reg <= 127;
                    CHECK_EQ(
                            goes_from_positive_to_negative || goes_negative_to_positive,
                            flag_reg.is_parity_overflow_flag_set()
                    );
                }
            }
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x80;

            add(acc_reg, 0x80, flag_reg);

            CHECK_EQ(0x0, acc_reg);
            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not set the carry flag when not carried out of msb") {
            Flags flag_reg;
            acc_reg = 0x1;

            add(acc_reg, 0x2, flag_reg);

            CHECK_EQ(0x3, acc_reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set the half carry flag when carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xF;

            add(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0x10, acc_reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should not set the half carry flag when not carried out of the fourth bit") {
            Flags flag_reg;
            acc_reg = 0xe;

            add(acc_reg, 0x1, flag_reg);

            CHECK_EQ(0xF, acc_reg);
            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always reset the add/subtract carry flag") {
            Flags flag_reg;
            acc_reg = 0xe;

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add(acc_reg, 0x1, flag_reg);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add(acc_reg, 0x1, flag_reg);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add(acc_reg, 0x1, flag_reg);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }
    }

    TEST_CASE("Z80: ADD A, r") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;

            add_A_r(acc_reg, 0x1, flag_reg, cycles);
            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: ADD A, n") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;
            NextByte args = {0x1};

            add_A_n(acc_reg, args, flag_reg, cycles);
            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: ADD A, [HL]") {
        SUBCASE("should use 7 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 acc_reg = 0xe;

            add_A_MHL(acc_reg, 0x1, flag_reg, cycles);
            CHECK_EQ(7, cycles);
        }
    }

    TEST_CASE("Z80: ADD HL, ss") {
        unsigned long cycles = 0;

        SUBCASE("should add the given value to HL") {
            for (u16 hl_counter = 0; hl_counter < 100; ++hl_counter) {
                for (u16 value_to_add = 0; value_to_add < UINT16_MAX; ++value_to_add) {
                    Flags flag_reg;

                    u8 h_reg = second_byte(hl_counter);
                    u8 l_reg = first_byte(hl_counter);

                    add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

                    CHECK_EQ(
                            static_cast<u16>(hl_counter + value_to_add),
                            to_u16(h_reg, l_reg)
                    );
                }
            }
        }

        SUBCASE("should set the carry flag when carried out of msb") {
            Flags flag_reg;

            u8 h_reg = 0xff;
            u8 l_reg = 0xff;
            u16 value_to_add = 0x1;

            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

//        SUBCASE("should set the half carry flag when carried out of the 11th bit") { // TODO: Figure out half carry for dad
//            Flags flag_reg;
//
//            u8 h_reg = 0x08;
//            u8 l_reg = 0x00;
//            u16 value_to_add = 0x1;
//
//            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
//
//            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);
//
//            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
//        }

        SUBCASE("should always reset the add/subtract carry flag") {
            Flags flag_reg;
            u8 h_reg = 0xff;
            u8 l_reg = 0xff;
            u16 value_to_add = 0x40;

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should use 11 cycles") {
            cycles = 0;
            Flags flag_reg;

            u8 h_reg = 0;
            u8 l_reg = 0xE;
            u16 value_to_add = 0x4;

            add_HL_ss(h_reg, l_reg, value_to_add, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
