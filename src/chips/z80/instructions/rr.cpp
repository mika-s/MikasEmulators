#include <iostream>
#include "doctest.h"
#include "chips/z80/flags.h"
#include "instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;

    void rr(u8 &value, Flags &flag_reg) {
        const bool should_set_carry_flag = is_bit_set(value, lsb);
        value = value >> 1;
        if (flag_reg.is_carry_flag_set()) {
            set_bit(value, msb);
        }
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
        } else {
            flag_reg.clear_carry_flag();
        }

        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(value);
    }

    /**
     * Rotate right through carry (accumulator)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rra(u8 &acc_reg, Flags &flag_reg, cyc &cycles) {
        rr(acc_reg, flag_reg);

        cycles = 4;
    }

    /**
     * Rotate right through carry (register)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to rotate, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rr_r(u8 &reg, Flags &flag_reg, cyc &cycles) {
        rr(reg, flag_reg);

        flag_reg.handle_zero_flag(reg);
        flag_reg.handle_sign_flag(reg);
        flag_reg.handle_parity_flag(reg);

        cycles = 8;
    }

    /**
     * Rotate right through carry (value in memory at HL's address)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param value_in_hl is the value in memory at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rr_MHL(EmulatorMemory &memory, u16 address, Flags &flag_reg, cyc &cycles) {
        u8 value = memory.read(address);
        rr(value, flag_reg);
        memory.write(address, value);

        flag_reg.handle_zero_flag(value);
        flag_reg.handle_sign_flag(value);
        flag_reg.handle_parity_flag(value);

        cycles = 15;
    }

    /**
     * Rotate right through carry (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 6</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d is the address offset
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rr_MixyPd(u16 ixy_reg, u8 d, EmulatorMemory &memory, Flags &flag_reg, cyc &cycles) {
        const u16 address = ixy_reg + static_cast<i8>(d);
        u8 value = memory.read(address);

        rr(value, flag_reg);

        memory.write(address, value);

        flag_reg.handle_zero_flag(value);
        flag_reg.handle_sign_flag(value);
        flag_reg.handle_parity_flag(value);

        cycles = 23;
    }

    void print_rra(std::ostream &ostream) {
        ostream << "RRA";
    }

    void print_rr(std::ostream &ostream, const std::string &reg) {
        ostream << "RR "
                << reg;
    }

    void print_rr_MixyPn(std::ostream &ostream, const std::string &ixy_reg, u8 d) {
        const i8 signed_value = static_cast<i8>(d);
        const std::string plus_or_minus = (signed_value >= 0) ? "+" : "";

        ostream << "RR "
                << "("
                << ixy_reg
                << plus_or_minus
                << +signed_value
                << ")";
    }

    TEST_CASE("Z80: RRA") {
        cyc cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should rotate the accumulator right") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;

                rra(acc_reg, flag_reg, cycles);

                CHECK_EQ(static_cast<u8>(acc_reg_counter >> 1u), acc_reg);
            }
        }

        SUBCASE("should set the carry flag if shifted out of lsb") {
            acc_reg = 0b00000001;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should take the carry flag into account when shifting") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(0b10000000, acc_reg);
        }

        SUBCASE("should always reset the half carry flag") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_half_carry_flag();

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always reset the add/subtract flag") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;

            Flags flag_reg;
            flag_reg.set_carry_flag();

            rra(acc_reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
