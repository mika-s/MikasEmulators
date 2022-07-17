#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instructions.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    void dec(u8 &reg, Flags &flag_reg) {
        const u8 previous = reg;
        reg--;

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_overflow_flag(previous, -1, false);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_half_borrow_flag(previous, 1, false);
        flag_reg.set_add_subtract_flag();
    }

    /**
     * Decrement register
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dec_r(u8 &reg, Flags &flag_reg, unsigned long &cycles) {
        dec(reg, flag_reg);

        cycles = 4;
    }

    /**
     * Decrement the memory in HL's address
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
    void dec_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles) {
        dec(value_in_hl, flag_reg);

        cycles = 11;
    }

    /**
     * Decrement register pair
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
    void dec_ss(u8 &reg1, u8 &reg2, unsigned long &cycles) {
        u16 val = to_u16(reg1, reg2);
        --val;

        reg2 = first_byte(val);
        reg1 = second_byte(val);

        cycles = 6;
    }

    /**
     * Decrement register pair (SP)
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
    void dec_sp(u16 &sp, unsigned long &cycles) {
        --sp;

        cycles = 6;
    }

    /**
     * Decrement register (IX or IY)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dec_ix_iy(u16 &ix_or_iy, unsigned long &cycles) {
        --ix_or_iy;

        cycles = 10;
    }


    void print_dec(std::ostream &ostream, const std::string &reg) {
        ostream << "DEC "
                << reg;
    }

    TEST_CASE("Z80: DEC") {
        unsigned long cycles = 0;

        SUBCASE("should decrease register or memory") {
            u8 reg = 10;
            Flags flag_reg;

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(9, reg);

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(8, reg);

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(7, reg);

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(6, reg);

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(5, reg);
        }

        SUBCASE("should not affect the carry flag") {
            u8 reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(255, reg);
            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should set correct half carry flag") {
            u8 reg = 15;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(14, reg);
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should set the overflow flag when overflowing and not otherwise") {
            u8 acc_reg;

            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;

                dec(acc_reg, flag_reg);

                CHECK_EQ( // The manual: P/V is set if m was 80h before operation; otherwise, it is reset.
                        acc_reg_counter == 0x80,
                        flag_reg.is_parity_overflow_flag_set()
                );
            }
        }

        SUBCASE("should set correct value in the zero flag") {
            u8 reg = 2;
            Flags flag_reg;

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_zero_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_zero_flag_set());
        }

        SUBCASE("should set the sign flag when going above 127") {
            u8 reg = 0;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_sign_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_sign_flag_set());
        }

        SUBCASE("should always set the add/subtract flag") {
            u8 reg = 255;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(254, reg);
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(253, reg);
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(252, reg);
            CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
        }
    }

    TEST_CASE("Z80: DEC r") {
        SUBCASE("should use 4 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: DEC ss") {
        SUBCASE("should use 6 cycles") {
            unsigned long cycles = 0;
            u8 reg1 = UINT8_MAX;
            u8 reg2 = UINT8_MAX;
            u16 sp = UINT8_MAX;

            dec_ss(reg1, reg2, cycles);

            CHECK_EQ(6, cycles);

            cycles = 0;

            dec_sp(sp, cycles);

            CHECK_EQ(6, cycles);
        }
    }

    TEST_CASE("Z80: DEC (HL)") {
        SUBCASE("should use 11 cycles") {
            unsigned long cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            dec_MHL(reg, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: DEC (IX or IY)") {
        unsigned long cycles = 0;
        u16 ix = 0;

        SUBCASE("should decrease IX") {
            for (u16 expected_ix = UINT16_MAX; expected_ix > 0; --expected_ix) {
                ix = expected_ix;

                dec_ix_iy(ix, cycles);

                CHECK_EQ(expected_ix - 1, ix);
            }
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            dec_ix_iy(ix, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
