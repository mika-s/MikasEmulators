#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    /**
     * Increment register pair
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register in the register pair, which will be mutated
     * @param reg2 is the second register in the register pair, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inx(u8 &reg1, u8 &reg2, unsigned long &cycles) {
        u16 val = to_u16(reg1, reg2);
        ++val;

        reg2 = first_byte(val);
        reg1 = second_byte(val);

        cycles = 5;
    }

    /**
     * Increment register pair (SP)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void inx_sp(u16 &sp, unsigned long &cycles) {
        ++sp;

        cycles = 5;
    }

    void print_inx(std::ostream &ostream, const std::string &reg) {
        ostream << "INX "
                << reg;
    }

    TEST_CASE("Z80: INX") {
        unsigned long cycles = 0;
        u8 reg1 = 0;
        u8 reg2 = 0;
        u8 expected_reg1 = 0;
        u8 expected_reg2;
        u16 sp = 0;

        SUBCASE("should increase register pair") {
            for (int i = 0; i < UINT16_MAX; ++i) {
                inx(reg1, reg2, cycles);

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

                inx_sp(sp, cycles);

                CHECK_EQ(expected_sp + 1, sp);
            }
        }

        SUBCASE("should use 5 cycles") {
            cycles = 0;

            inx(reg1, reg2, cycles);

            CHECK_EQ(5, cycles);

            inx_sp(sp, cycles);

            CHECK_EQ(5, cycles);
        }
    }
}
