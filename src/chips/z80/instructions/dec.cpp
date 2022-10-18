#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::borrow_from;
    using emu::util::byte::low_byte;
    using emu::util::byte::high_byte;
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
    void dec_r(u8 &reg, Flags &flag_reg, cyc &cycles) {
        dec_u8(reg, flag_reg);

        cycles = 4;
    }

    /**
     * Decrement register (undocumented)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: half carry, zero, sign, parity, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to decrement, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dec_r_undoc(u8 &reg, Flags &flag_reg, cyc &cycles) {
        dec_u8(reg, flag_reg);

        cycles = 8;
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
    void dec_MHL(u8 &value_in_hl, Flags &flag_reg, cyc &cycles) {
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
    void dec_ss(u8 &reg1, u8 &reg2, cyc &cycles) {
        u16 val = to_u16(reg1, reg2);
        --val;

        reg2 = low_byte(val);
        reg1 = high_byte(val);

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
    void dec_sp(u16 &sp, cyc &cycles) {
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
    void dec_ixy(u16 &ixy_reg, cyc &cycles) {
        --ixy_reg;

        cycles = 10;
    }

    /**
     * Decrement IXH/IYH
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
    void dec_ixyh(u16 &ixy_reg, Flags &flag_reg, cyc &cycles) {
        u8 ixyh = high_byte(ixy_reg);
        const u8 ixyl = low_byte(ixy_reg);

        dec_u8(ixyh, flag_reg);

        ixy_reg = to_u16(ixyh, ixyl);

        cycles = 8;
    }

    /**
     * Decrement IXL/IYL
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
    void dec_ixyl(u16 &ixy_reg, Flags &flag_reg, cyc &cycles) {
        const u8 ixyh = high_byte(ixy_reg);
        u8 ixyl = low_byte(ixy_reg);

        dec_u8(ixyl, flag_reg);

        ixy_reg = to_u16(ixyh, ixyl);

        cycles = 8;
    }

    /**
     * Decrement value in memory pointed to by IX or IY plus d
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
    void dec_MixyPd(u16 ixy_reg, const NextByte &args, EmulatorMemory &memory, Flags &flag_reg, cyc &cycles) {
        dec_u8(memory[ixy_reg + static_cast<i8>(args.farg)], flag_reg);

        cycles = 23;
    }

    void print_dec(std::ostream &ostream, const std::string &reg) {
        ostream << "DEC "
                << reg;
    }

    void print_dec_undocumented(std::ostream &ostream, const std::string &reg) {
        ostream << "DEC "
                << reg
                << "*";
    }

    void print_dec_MixyPn(std::ostream &ostream, const std::string &ixy_reg, const NextByte &args) {
        const i8 signed_value = static_cast<i8>(args.farg);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "DEC "
                << "("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
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
            cyc cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            dec_r(reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }

    TEST_CASE("Z80: DEC ss") {
        SUBCASE("should use 6 cycles") {
            cyc cycles = 0;
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
            cyc cycles = 0;
            Flags flag_reg;
            u8 reg = UINT8_MAX;

            dec_MHL(reg, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: DEC (IX or IY)") {
        cyc cycles = 0;
        u16 ix = 0;

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            dec_ixy(ix, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
