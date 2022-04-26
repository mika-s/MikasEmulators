#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
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
    void dcx(std::uint8_t &reg1, std::uint8_t &reg2, unsigned long &cycles) {
        std::uint16_t val = emu::util::byte::to_u16(reg1, reg2);
        --val;

        reg2 = emu::util::byte::first_byte(val);
        reg1 = emu::util::byte::second_byte(val);

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
    void dcx_sp(std::uint16_t &sp, unsigned long &cycles) {
        --sp;

        cycles = 5;
    }

    void print_dcx(std::ostream &ostream, const std::string &reg) {
        ostream << "DCX "
                << reg;
    }

    TEST_CASE("8080: DCX") {
        unsigned long cycles = 0;
        std::uint8_t reg1 = UINT8_MAX;
        std::uint8_t reg2 = UINT8_MAX;
        std::uint8_t expected_reg1 = UINT8_MAX;
        std::uint8_t expected_reg2;
        std::uint16_t sp = UINT16_MAX;

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
            for (std::uint16_t expected_sp = UINT16_MAX; expected_sp > 0; --expected_sp) {
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
