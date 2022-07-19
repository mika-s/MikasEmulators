#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    void inc(u8 &reg, Flags &flag_reg) {
        const u8 previous = reg;
        reg++;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_overflow_flag(previous, 1, false);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_half_carry_flag(previous, 1, false);
        flag_reg.clear_add_subtract_flag();
    }

    /**
     * Increment register
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to increment, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_r(u8 &reg, Flags &flag_reg, unsigned long &cycles) {
        inc(reg, flag_reg);

        cycles = 4;
    }

    /**
     * Increment the memory in HL's address
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity, add/subtract</li>
     * </ul>
     *
     * @param value_in_hl is the value in memory at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles) {
        inc(value_in_hl, flag_reg);

        cycles = 11;
    }

    /**
     * Increment register pair
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 6</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register in the register pair, which will be mutated
     * @param reg2 is the second register in the register pair, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_ss(u8 &reg1, u8 &reg2, unsigned long &cycles) {
        u16 val = to_u16(reg1, reg2);
        ++val;

        reg2 = first_byte(val);
        reg1 = second_byte(val);

        cycles = 6;
    }

    /**
     * Increment register pair (SP)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 6</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_sp(u16 &sp, unsigned long &cycles) {
        ++sp;

        cycles = 6;
    }

    /**
     * Increment IX or IY register
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_ixy(u16 &ixy_reg, unsigned long &cycles) {
        ++ixy_reg;

        cycles = 10;
    }

    void print_inc(std::ostream &ostream, const std::string &reg) {
        ostream << "INC "
                << reg;
    }

    TEST_CASE("Z80: INC") {
        SUBCASE("should increase register or memory") {
            u8 reg = 0;
            Flags flag_reg;

            inc(reg, flag_reg);

            CHECK_EQ(1, reg);

            inc(reg, flag_reg);

            CHECK_EQ(2, reg);

            inc(reg, flag_reg);

            CHECK_EQ(3, reg);

            inc(reg, flag_reg);

            CHECK_EQ(4, reg);

            inc(reg, flag_reg);

            CHECK_EQ(5, reg);
        }

        SUBCASE("should not affect the carry flag") {
            u8 reg = 255;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            inc(reg, flag_reg);

            CHECK_EQ(0, reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set correct half carry flag") {
            u8 reg = 15;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            inc(reg, flag_reg);

            CHECK_EQ(16, reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should set correct value in the zero flag") {
            u8 reg = 254;
            Flags flag_reg;

            inc(reg, flag_reg);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            inc(reg, flag_reg);

            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when going above 127") {
            u8 reg = 127;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_sign_flag_set());

            inc(reg, flag_reg);

            CHECK_EQ(true, flag_reg.is_sign_flag_set());
        }

        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
            u8 acc_reg;

            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;

                inc(acc_reg, flag_reg);

                CHECK_EQ( // The manual: P/V is set if r was 7Fh before operation; otherwise, it is reset.
                        acc_reg_counter == 0x7f,
                        flag_reg.is_parity_overflow_flag_set()
                );
            }
        }

        SUBCASE("should always unset the add/subtract flag") {
            u8 reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            inc(reg, flag_reg);

            CHECK_EQ(1, reg);
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            flag_reg.set_add_subtract_flag();

            inc(reg, flag_reg);

            CHECK_EQ(2, reg);
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            flag_reg.set_add_subtract_flag();

            inc(reg, flag_reg);

            CHECK_EQ(3, reg);
            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }
    }

    TEST_CASE("Z80: INC r") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            inc_r(reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: INC ss") {
        unsigned long cycles = 0;
        u8 reg1 = 0;
        u8 reg2 = 0;
        u8 expected_reg1 = 0;
        u8 expected_reg2;
        u16 sp = 0;

        SUBCASE("should increase register pair") {
            for (int i = 0; i < UINT16_MAX; ++i) {
                inc_ss(reg1, reg2, cycles);

                if (reg2 % (UINT8_MAX + 1) == 0 && i != 0) {
                    ++expected_reg1;
                }

                expected_reg2 = i + 1;

                CHECK_EQ(expected_reg1, reg1);
                CHECK_EQ(expected_reg2, reg2);
            }
        }

        SUBCASE("should increase SP") {
            for (u16 expected_sp = 0; expected_sp < UINT16_MAX; ++expected_sp) {
                sp = expected_sp;

                inc_sp(sp, cycles);

                CHECK_EQ(expected_sp + 1, sp);
            }
        }

        SUBCASE("should use 6 cycles") {
            cycles = 0;

            inc_ss(reg1, reg2, cycles);

            CHECK_EQ(6, cycles);

            inc_sp(sp, cycles);

            CHECK_EQ(6, cycles);
        }
    }

    TEST_CASE("Z80: INC (HL)") {
        SUBCASE("should use 11 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            inc_MHL(reg, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: INC IX or INC IY)") {
        unsigned long cycles = 0;
        u16 ix = 0;

        SUBCASE("should increase IX") {
            for (u16 expected_ix = 0; expected_ix < UINT16_MAX; ++expected_ix) {
                ix = expected_ix;

                inc_ixy(ix, cycles);

                CHECK_EQ(expected_ix + 1, ix);
            }
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            inc_ixy(ix, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
