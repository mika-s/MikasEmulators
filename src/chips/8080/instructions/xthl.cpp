#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;
    using emu::util::byte::low_byte;
    using emu::util::byte::high_byte;
    using emu::util::byte::to_u16;

    void ex_msp_dd(u16 sp, EmulatorMemory &memory, u16 &reg) {
        const u16 previous_reg = reg;
        reg = to_u16(memory.read(sp + 1), memory.read(sp));
        memory.write(sp, low_byte(previous_reg));
        memory.write(sp + 1, high_byte(previous_reg));
    }

    /**
     * Exchange H&L with top of stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 18</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param sp0 is the top of the stack, which will be mutated
     * @param sp1 is the second from the top of the stack, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xthl(u16 sp, EmulatorMemory &memory, u8 &h_reg, u8 &l_reg, cyc &cycles) {
        u16 hl = to_u16(h_reg, l_reg);
        ex_msp_dd(sp, memory, hl);
        h_reg = high_byte(hl);
        l_reg = low_byte(hl);

        cycles = 18;
    }

    void print_xthl(std::ostream &ostream) {
        ostream << "XTHL";
    }

    TEST_CASE("8080: XTHL") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add({0x33, 0x44});

        SUBCASE("should exchange HL with top of the stack") {
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u16 sp = 0x0000;

            xthl(sp, memory, h_reg, l_reg, cycles);

            CHECK_EQ(0x44, h_reg);
            CHECK_EQ(0x33, l_reg);
            CHECK_EQ(0x22, memory.read(sp));
            CHECK_EQ(0x11, memory.read(sp + 1));
        }

        SUBCASE("should use 18 cycles") {
            cycles = 0;
            u8 h_reg = 0x11;
            u8 l_reg = 0x22;
            u16 sp = 0x0000;

            xthl(sp, memory, h_reg, l_reg, cycles);

            CHECK_EQ(18, cycles);
        }
    }
}
