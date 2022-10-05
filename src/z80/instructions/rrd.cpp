#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::is_bit_set;
    using emu::util::byte::set_bit;

    /**
     * Rotate right digit
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 18</li>
     *   <li>Condition bits affected: half carry, sign, zero, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param value is the value at HL's address, which will be mutated
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rrd(u8 &acc_reg, u8 &value, Flags &flag_reg, cyc &cycles) {
        u8 new_acc = acc_reg;

        new_acc = (new_acc & 0xf0) | (value & 0x0f);
        value = ((acc_reg & 0x0f) << 4) | ((value & 0xf0) >> 4);

        acc_reg = new_acc;

        flag_reg.clear_half_carry_flag();
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);
        flag_reg.clear_add_subtract_flag();
        flag_reg.handle_xy_flags(acc_reg);

        cycles = 18;
    }

    void print_rrd(std::ostream &ostream) {
        ostream << "RRD";
    }

    TEST_CASE("Z80: RRD") {
        cyc cycles = 0;
        u8 acc_reg = 0;
        u8 value = 0;

        SUBCASE("should put (HL) LS nibble in LS nibble acc_reg, and LS nibble in acc_reg in MS nibble (HL), and MS nibble (HL) in LS nibble (HL)") {
            Flags flag_reg;
            acc_reg = 0x84;
            value = 0x20;

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(0x80, acc_reg);
            CHECK_EQ(0x42, value);
        }

        SUBCASE("should ignore the carry flag into account when shifting") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            acc_reg = 0xab;
            value = 0xde;

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());

            flag_reg.clear_carry_flag();

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_carry_flag_set());
        }

        SUBCASE("should always reset the half carry flag") {
            Flags flag_reg;
            flag_reg.set_half_carry_flag();
            acc_reg = 0xab;
            value = 0xde;

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());

            flag_reg.set_half_carry_flag();

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_half_carry_flag_set());
        }

        SUBCASE("should always reset the add/subtract flag") {
            Flags flag_reg;
            flag_reg.set_add_subtract_flag();
            acc_reg = 0xab;
            value = 0xde;

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());

            flag_reg.set_add_subtract_flag();

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(false, flag_reg.is_add_subtract_flag_set());
        }

        SUBCASE("should use 18 cycles") {
            cycles = 0;

            Flags flag_reg;
            acc_reg = 0xab;
            value = 0xde;

            rrd(acc_reg, value, flag_reg, cycles);

            CHECK_EQ(18, cycles);
        }
    }
}
