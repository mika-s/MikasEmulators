#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::low_byte;
    using emu::util::byte::high_byte;
    using emu::util::byte::to_u16;

    void inc(u8 &reg, Flags &flag_reg) {
        const bool old_carry = flag_reg.is_carry_flag_set();

        add_to_register(reg, 1, false, flag_reg);

        if (old_carry) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }
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
    void inc_r(u8 &reg, Flags &flag_reg, cyc &cycles) {
        inc(reg, flag_reg);

        cycles = 4;
    }

    /**
     * Increment register (undocumented)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to increment, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_r_undoc(u8 &reg, Flags &flag_reg, cyc &cycles) {
        inc(reg, flag_reg);

        cycles = 8;
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
    void inc_MHL(u8 &value_in_hl, Flags &flag_reg, cyc &cycles) {
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
    void inc_ss(u8 &reg1, u8 &reg2, cyc &cycles) {
        u16 val = to_u16(reg1, reg2);
        ++val;

        reg2 = low_byte(val);
        reg1 = high_byte(val);

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
    void inc_sp(u16 &sp, cyc &cycles) {
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
    void inc_ixy(u16 &ixy_reg, cyc &cycles) {
        ++ixy_reg;

        cycles = 10;
    }

    /**
     * Increment IXH/IYH
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_ixyh(u16 &ixy_reg, Flags &flag_reg, cyc &cycles) {
        u8 ixyh = high_byte(ixy_reg);
        const u8 ixyl = low_byte(ixy_reg);

        inc(ixyh, flag_reg);

        ixy_reg = to_u16(ixyh, ixyl);

        cycles = 8;
    }

    /**
     * Increment IXL/IYL
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_ixyl(u16 &ixy_reg, Flags &flag_reg, cyc &cycles) {
        const u8 ixyh = high_byte(ixy_reg);
        u8 ixyl = low_byte(ixy_reg);

        inc(ixyl, flag_reg);

        ixy_reg = to_u16(ixyh, ixyl);

        cycles = 8;
    }

    /**
     * Increment value in memory pointed to by IX or IY plus d
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register containing the base address
     * @param args contains address offset
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inc_MixyPd(u16 ixy_reg, const NextByte &args, EmulatorMemory &memory, Flags &flag_reg, cyc &cycles) {
        inc(memory[ixy_reg + static_cast<i8>(args.farg)], flag_reg);

        cycles = 23;
    }

    void print_inc(std::ostream &ostream, const std::string &reg) {
        ostream << "INC "
                << reg;
    }

    void print_inc_undocumented(std::ostream &ostream, const std::string &reg) {
        ostream << "INC "
                << reg
                << "*";
    }

    void print_inc_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args) {
        const i8 signed_value = static_cast<i8>(args.farg);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "INC "
                << "("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
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
            u8 reg = INT8_MAX;
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
            cyc cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            inc_r(reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: INC ss") {
        cyc cycles = 0;
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
            cyc cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            inc_MHL(reg, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: INC IX or INC IY)") {
        cyc cycles = 0;
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
