#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::borrow_from;
    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    void dec_u8(u8 &reg, Flags &flag_reg) {
        const bool old_carry = flag_reg.is_carry_flag_set();

        sub_from_register(reg, 1, false, flag_reg);

        if (old_carry) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }
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
        dec_u8(reg, flag_reg);

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
        dec_u8(value_in_hl, flag_reg);

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
     * @param ixy_reg is either the IX or IY register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dec_ixy(u16 &ixy_reg, unsigned long &cycles) {
        --ixy_reg;

        cycles = 10;
    }

    void print_dec(std::ostream &ostream, const std::string &reg) {
        ostream << "DEC "
                << reg;
    }

    TEST_CASE("Z80: DEC (8-bit)") {
        SUBCASE("should decrease value by 1") {
            for (u8 reg_counter = 0; reg_counter < UINT8_MAX; ++reg_counter) {
                Flags flag_reg;
                u8 reg = reg_counter;

                dec_u8(reg, flag_reg);

                CHECK_EQ(static_cast<u8>(reg_counter - 1), reg);
                CHECK_EQ(false, flag_reg.is_carry_flag_set());
                CHECK_EQ( // The manual: P/V is set if m was 80h before operation; otherwise, it is reset.
                        reg_counter == 0x80,
                        flag_reg.is_parity_overflow_flag_set()
                );
                CHECK_EQ(reg == 0, flag_reg.is_zero_flag_set());
                CHECK_EQ(static_cast<i8>(reg) < 0, flag_reg.is_sign_flag_set());
                CHECK_EQ(true, flag_reg.is_add_subtract_flag_set());
                CHECK_EQ(borrow_from(4, reg_counter, 1, false), flag_reg.is_half_carry_flag_set());
            }
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

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            dec_ixy(ix, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
