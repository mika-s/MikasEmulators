#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;

    void rrc(u8 &value, Flags &flag_reg) {
        const bool should_set_carry_flag = is_bit_set(value, lsb);
        value = value >> 1;
        if (should_set_carry_flag) {
            flag_reg.set_carry_flag();
            set_bit(value, msb);
        } else {
            flag_reg.clear_carry_flag();
        }

        flag_reg.clear_half_carry_flag();
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(value);
    }

    /**
     * Rotate right circular (accumulator)
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
    void rrca(u8 &acc_reg, Flags &flag_reg, unsigned long &cycles) {
        rrc(acc_reg, flag_reg);

        cycles = 4;
    }

    /**
     * Rotate right circular (register)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 8</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param reg is the register to rotate, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rrc_r(u8 &reg, Flags &flag_reg, unsigned long &cycles) {
        rrc(reg, flag_reg);

        cycles = 8;
    }

    /**
     * Rotate right circular (the value in memory at HL's address)
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param value_in_hl is the value in memory at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rrc_MHL(u8 &value_in_hl, Flags &flag_reg, unsigned long &cycles) {
        rrc(value_in_hl, flag_reg);

        cycles = 15;
    }

    /**
     * Rotate right circular (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d is the address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rrc_MixyPd(u16 ixy_reg, u8 d, const EmulatorMemory &memory, Flags &flag_reg,
                    unsigned long &cycles
    ) {
        u8 value = memory[ixy_reg + static_cast<i8>(d)];
        rrc(value, flag_reg);

        cycles = 23;
    }

    /**
     * Rotate right circular (value in memory pointed to by IX or IY plus d)
     * <ul>
     *   <li>Size: 4</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 23</li>
     *   <li>Condition bits affected: carry, half carry, add/subtract</li>
     * </ul>
     *
     * @param reg is the register store the result in, which will be mutated
     * @param ixy_reg is the IX or IY register containing the base address
     * @param d is the address offset
     * @param memory is the memory
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rrc_MixyPd_r(u8 &reg, u16 ixy_reg, u8 d, const EmulatorMemory &memory, Flags &flag_reg,
                      unsigned long &cycles
    ) {
        u8 value = memory[ixy_reg + static_cast<i8>(d)];
        rrc(value, flag_reg);
        reg = value;

        cycles = 23;
    }

    void print_rrca(std::ostream &ostream) {
        ostream << "RRCA";
    }

    void print_rrc(std::ostream &ostream, const std::string &reg) {
        ostream << "RLC "
                << reg;
    }

    TEST_CASE("Z80: RRCA") {
        unsigned long cycles = 0;
        u8 acc_reg = 0;

        SUBCASE("should rotate the accumulator right") {
            for (u8 acc_reg_counter = 0; acc_reg_counter < UINT8_MAX; ++acc_reg_counter) {
                Flags flag_reg;
                acc_reg = acc_reg_counter;
                const bool cy = is_bit_set(acc_reg, lsb);

                rrca(acc_reg, flag_reg, cycles);

                u8 expected = acc_reg_counter >> 1u;
                if (cy) {
                    set_bit(expected, msb);
                }

                CHECK_EQ(expected, acc_reg);
            }
        }

        SUBCASE("should set the carry flag if shifted out of lsb") {
            acc_reg = 0b00000001;
            Flags flag_reg;

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should not take the carry flag into account when shifting") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(0, acc_reg);
        }

        SUBCASE("should always reset the half carry flag") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_half_carry_flag();

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always reset the add/subtract flag") {
            acc_reg = 0;
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should use 4 cycles") {
            cycles = 0;
            Flags flag_reg;
            flag_reg.set_carry_flag();

            rrca(acc_reg, flag_reg, cycles);

            CHECK_EQ(4, cycles);
        }
    }
}
