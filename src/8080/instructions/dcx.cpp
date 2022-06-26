#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;
    using emu::util::byte::to_u16;

    /**
     * Decrement register pair
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
    void dcx(u8 &reg1, u8 &reg2, unsigned long &cycles) {
        u16 val = to_u16(reg1, reg2);
        --val;

        reg2 = first_byte(val);
        reg1 = second_byte(val);

        cycles = 5;
    }

    /**
     * Decrement register pair (SP)
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
    void dcx_sp(u16 &sp, unsigned long &cycles) {
        --sp;

        cycles = 5;
    }

    void print_dcx(std::ostream &ostream, const std::string &reg) {
        ostream << "DCX "
                << reg;
    }

    TEST_CASE("8080: DCX") {
        unsigned long cycles = 0;
        u8 reg1 = UINT8_MAX;
        u8 reg2 = UINT8_MAX;
        u8 expected_reg1 = UINT8_MAX;
        u8 expected_reg2;
        u16 sp = UINT16_MAX;

        SUBCASE("should decrease register pair") {
            for (int i = UINT16_MAX; i > UINT16_MAX; --i) {
                if (reg2 == 0 && i != 0) {
                    --expected_reg1;
                }

                expected_reg2 = i - 1;

                dcx(reg1, reg2, cycles);

                CHECK_EQ(expected_reg1, reg1);
                CHECK_EQ(expected_reg2, reg2);
            }
        }

        SUBCASE("should decrease SP") {
            for (u16 expected_sp = UINT16_MAX; expected_sp > 0; --expected_sp) {
                sp = expected_sp;
                dcx_sp(sp, cycles);

                CHECK_EQ(expected_sp - 1, sp);
            }
        }

        SUBCASE("should use 5 cycles") {
            cycles = 0;

            dcx(reg1, reg2, cycles);

            CHECK_EQ(5, cycles);

            cycles = 0;

            dcx_sp(sp, cycles);

            CHECK_EQ(5, cycles);
        }
    }
}
