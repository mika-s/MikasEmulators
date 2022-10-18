#include <iostream>
#include "doctest.h"
#include "instructions.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080 {
    /**
     * Move
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param value is the value to move into to_reg
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mov(u8 &to, u8 value, cyc &cycles) {
        mov(to, value, cycles, false);
    }

    /**
     * Move
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param value is the value to move into to_reg
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void mov(u8 &to, u8 value, cyc &cycles, bool is_memory_involved) {
        to = value;

        cycles = 5;

        if (is_memory_involved) {
            cycles += 2;
        }
    }

    void print_mov(std::ostream &ostream, const std::string &reg1, const std::string &reg2) {
        ostream << "MOV "
                << reg1 << ","
                << reg2;
    }

    TEST_CASE("8080: MOV") {
        cyc cycles = 0;

        SUBCASE("should load registers with value") {
            u8 reg1 = 0;
            u8 reg2 = 0;

            mov(reg1, reg2, cycles);
            CHECK_EQ(0, reg1);

            reg2 = 0xa;
            mov(reg1, reg2, cycles);
            CHECK_EQ(0xa, reg1);

            reg2 = 0xff;
            mov(reg1, reg2, cycles);
            CHECK_EQ(0xff, reg1);
        }

        SUBCASE("should load the memory with value") {
            u8 reg1 = 0;
            u8 reg2 = 0;

            mov(reg1, reg2, cycles, true);
            CHECK_EQ(0, reg1);

            reg2 = 0xa;
            mov(reg1, reg2, cycles, true);
            CHECK_EQ(0xa, reg1);

            reg2 = 0xff;
            mov(reg1, reg2, cycles, true);
            CHECK_EQ(0xff, reg1);
        }

        SUBCASE("should use 5 cycles if memory is not involved") {
            cycles = 0;
            u8 reg1 = 0;
            u8 reg2 = 0x11;

            mov(reg1, reg2, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            u8 reg1 = 0;
            u8 reg2 = 0x11;

            mov(reg1, reg2, cycles, true);

            CHECK_EQ(7, cycles);
        }
    }
}
