#include "chips/z80/flags.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::z80 {

    using emu::memory::EmulatorMemory;
    using emu::util::byte::high_byte;
    using emu::util::byte::low_byte;

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
    void push_qq(u8 reg1, u8 reg2, u16 &sp, EmulatorMemory<u16, u8> &memory, cyc &cycles) {
        memory.write(--sp, reg1);
        memory.write(--sp, reg2);

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
    void push_af(const Flags &flag_reg, u8 acc_reg, u16 &sp, EmulatorMemory<u16, u8> &memory, cyc &cycles) {
        memory.write(--sp, acc_reg);
        memory.write(--sp, flag_reg.to_u8());

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
     * @param ixy_reg is the IX or IY register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push_ixy(u16 ixy_reg, u16 &sp, EmulatorMemory<u16, u8> &memory, cyc &cycles) {
        memory.write(--sp, high_byte(ixy_reg));
        memory.write(--sp, low_byte(ixy_reg));

        cycles = 15;
    }

    void print_push(std::ostream &ostream, const std::string &reg) {
        ostream << "PUSH "
                << reg;
    }

    TEST_CASE("Z80: PUSH qq") {
        cyc cycles = 0;

        SUBCASE("should push registers onto the stack") {
            u8 reg1 = 0xaa;
            u8 reg2 = 0xbb;
            u16 sp = 0x03;

            EmulatorMemory<u16, u8> memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_qq(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(reg1, memory.read(0x2));
            CHECK_EQ(reg2, memory.read(0x1));
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

            EmulatorMemory<u16, u8> memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_af(flag_reg, acc_reg, sp, memory, cycles);

            CHECK_EQ(acc_reg, memory.read(0x2));
            CHECK_EQ(flag_reg.to_u8(), memory.read(0x1));
            CHECK_EQ(0x01, sp);
        }

        SUBCASE("should use 11 cycles when pushing register pairs onto the stack") {
            cycles = 0;

            const u8 reg1 = 0;
            const u8 reg2 = 0;
            u16 sp = 0x03;

            EmulatorMemory<u16, u8> memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_qq(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(11, cycles);
        }
    }

    TEST_CASE("Z80: PUSH IX/IY") {
        cyc cycles = 0;

        SUBCASE("should use 15 cycles when pushing IX or IY onto the stack") {
            cycles = 0;

            const u16 ix = 0x01234;
            u16 sp = 0x03;

            EmulatorMemory<u16, u8> memory;
            memory.add(std::vector<u8>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_ixy(ix, sp, memory, cycles);

            CHECK_EQ(15, cycles);
        }
    }
}
