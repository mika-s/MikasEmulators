#include <iostream>
#include "doctest.h"
#include "z80/emulator_memory.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::z80 {

    using emu::util::byte::first_byte;
    using emu::util::byte::second_byte;

    void rst(u16 &pc, u16 new_pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {

        execute_call(pc, sp, memory, first_byte(new_pc), second_byte(new_pc));

        cycles = 11;
    }

    /**
     * Restart (address 0)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_0(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x00, sp, memory, cycles);
    }

    /**
     * Restart (address 1)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_1(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x08, sp, memory, cycles);
    }

    /**
     * Restart (address 2)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_2(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x10, sp, memory, cycles);
    }

    /**
     * Restart (address 3)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_3(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x18, sp, memory, cycles);
    }

    /**
     * Restart (address 4)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_4(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x20, sp, memory, cycles);
    }

    /**
     * Restart (address 5)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_5(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x28, sp, memory, cycles);
    }

    /**
     * Restart (address 6)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_6(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x30, sp, memory, cycles);
    }

    /**
     * Restart (address 7)
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rst_7(u16 &pc, u16 &sp, EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x38, sp, memory, cycles);
    }

    void print_rst(std::ostream &ostream, int number) {
        ostream << "RST "
                << number;
    }

    TEST_CASE("Z80: RST") {
        SUBCASE("should push PC onto the stack and change to the new PC -- 0") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x00;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_0(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 1") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x08;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_1(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 2") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x10;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_2(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 3") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x18;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_3(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 4") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x20;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_4(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 5") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x28;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_5(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 6") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x30;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_6(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should push PC onto the stack and change to the new PC -- 7") {
            unsigned long cycles = 0;

            u16 pc = 0xac12;
            u16 expected_new_pc = 0x38;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_7(pc, sp, memory, cycles);

            CHECK_EQ(expected_new_pc, pc);
            CHECK_EQ(0xac, memory[0x02]);
            CHECK_EQ(0x12, memory[0x01]);
        }

        SUBCASE("should use 11 cycles when returning") {
            unsigned long cycles = 0;

            u16 pc = 0;
            u16 sp = 0x03;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05});

            rst_0(pc, sp, memory, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
