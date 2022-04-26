#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
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
    void inx(std::uint8_t &reg1, std::uint8_t &reg2, unsigned long &cycles) {
        std::uint16_t val = emu::util::byte::to_u16(reg1, reg2);
        ++val;

        reg2 = emu::util::byte::first_byte(val);
        reg1 = emu::util::byte::second_byte(val);

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
    void inx_sp(std::uint16_t &sp, unsigned long &cycles) {
        ++sp;

        cycles = 5;
    }

    void print_inx(std::ostream &ostream, const std::string &reg) {
        ostream << "INX "
                << reg;
    }

    TEST_CASE("8080: INX") {
        unsigned long cycles = 0;
        std::uint8_t reg1 = 0;
        std::uint8_t reg2 = 0;
        std::uint8_t expected_reg1 = 0;
        std::uint8_t expected_reg2;
        std::uint16_t sp = 0;

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
            for (std::uint16_t expected_sp = 0; expected_sp < UINT16_MAX; ++expected_sp) {
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
