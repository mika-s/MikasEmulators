#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;

    /**
     * Push register pair onto the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to place in memory
     * @param reg2 is the second register to place in memory
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push_qq(u8 reg1, u8 reg2, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        memory[--sp] = reg1;
        memory[--sp] = reg2;

        cycles = 11;
    }

    /**
     * Push AF onto the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param flag_reg is the flag register
     * @param acc_reg is the accumulator register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push_af(const Flags &flag_reg, u8 acc_reg, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        memory[--sp] = acc_reg;
        memory[--sp] = flag_reg.to_u8();

        cycles = 11;
    }

    /**
     * Push IX or IY onto the stack
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 15</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to place in memory
     * @param reg1 is the second register to place in memory
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push_ix_iy(u16 ix_iy_reg, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        memory[--sp] = second_byte(ix_iy_reg);
        memory[--sp] = first_byte(ix_iy_reg);

        cycles = 15;
    }

    void print_push(std::ostream &ostream, const std::string &reg) {
        ostream << "PUSH "
                << reg;
    }

    TEST_CASE("Z80: PUSH qq") {
        unsigned long cycles = 0;

        SUBCASE("should push registers onto the stack") {
            u8 reg1 = 0xaa;
            u8 reg2 = 0xbb;
            u16 sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_qq(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(reg1, memory[0x2]);
            CHECK_EQ(reg2, memory[0x1]);
            CHECK_EQ(0x01, sp);
        }

        SUBCASE("should push AF onto the stack") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            flag_reg.set_zero_flag();
            flag_reg.set_sign_flag();
            flag_reg.set_parity_overflow_flag();
            flag_reg.set_half_carry_flag();
            u8 acc_reg = 0xbb;
            u16 sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_af(flag_reg, acc_reg, sp, memory, cycles);

            CHECK_EQ(acc_reg, memory[0x2]);
            CHECK_EQ(flag_reg.to_u8(), memory[0x1]);
            CHECK_EQ(0x01, sp);
        }

        SUBCASE("should use 11 cycles when pushing register pairs onto the stack") {
            cycles = 0;

            const u8 reg1 = 0;
            const u8 reg2 = 0;
            u16 sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_qq(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: PUSH IX/IY") {
        unsigned long cycles = 0;

        SUBCASE("should use 15 cycles when pushing IX or IY onto the stack") {
            cycles = 0;

            const u16 ix = 0x01234;
            u16 sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_ix_iy(ix, sp, memory, cycles);

            CHECK_EQ(15, cycles);
        }
    }
}
