#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::to_u16;

    /**
     * Pop register pair off the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to pop to, which will be mutated
     * @param reg2 is the second register to pop to, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop(u8 &reg1, u8 &reg2, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles) {
        reg2 = memory[sp++];
        reg1 = memory[sp++];

        cycles = 10;
    }

    /**
     * Pop AF off the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry, half carry, zero, sign, parity/overflow, add/subtract</li>
     * </ul>
     *
     * @param flag_reg is the flag register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop_af(Flags &flag_reg, u8 &acc_reg, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles) {
        flag_reg.from_u8(memory[sp++]);
        acc_reg = memory[sp++];

        cycles = 10;
    }

    /**
     * Pop IX or IY off the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 14</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param ix_reg is the IX register register, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop_ixy(u16 &ix_iy_reg, u16 &sp, const EmulatorMemory &memory, unsigned long &cycles) {
        const u8 lo = memory[sp++];
        const u8 hi = memory[sp++];

        ix_iy_reg = to_u16(hi, lo);

        cycles = 14;
    }

    void print_pop(std::ostream &ostream, const std::string &reg) {
        ostream << "POP "
                << reg;
    }

    TEST_CASE("Z80: POP qq") {
        unsigned long cycles = 0;

        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0xff, 0x05, 0x06, 0x07, 0x08});

        SUBCASE("should pop register from stack") {
            u8 reg1 = 0xaa;
            u8 reg2 = 0xbb;
            u16 sp = 0x03;

            pop(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(memory[0x03], reg2);
            CHECK_EQ(memory[0x04], reg1);
            CHECK_EQ(0x05, sp);
        }

        SUBCASE("should pop AF from the stack") {
            Flags flag_reg;
            u8 acc_reg = 0xaa;
            u16 sp = 0x03;

            pop_af(flag_reg, acc_reg, sp, memory, cycles);

            CHECK_EQ(true, flag_reg.is_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_zero_flag_set());
            CHECK_EQ(true, flag_reg.is_parity_overflow_flag_set());
            CHECK_EQ(true, flag_reg.is_half_carry_flag_set());
            CHECK_EQ(true, flag_reg.is_sign_flag_set());
            CHECK_EQ(0x05, acc_reg);
            CHECK_EQ(0x05, sp);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            u8 reg1 = 0;
            u8 reg2 = 0;
            u16 sp = 0x03;

            pop(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(10, cycles);
        }
    }

    TEST_CASE("Z80: POP IX/IY") {
        unsigned long cycles = 0;

        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0xff, 0x05, 0x06, 0x07, 0x08});

        SUBCASE("should use 14 cycles") {
            cycles = 0;

            u16 ix = 0x1234;
            u16 sp = 0x03;

            pop_ixy(ix, sp, memory, cycles);

            CHECK_EQ(14, cycles);
        }
    }
}
