#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "doctest.h"
#include "instructions.h"
#include <iostream>
#include <string>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;

    void mov(u8 &to, u8 value) {
        to = value;
    }

    /**
     * Move value to register
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param reg is the value to move into to
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mov_r_r(u8 &to, u8 value, cyc &cycles) {
        mov(to, value);

        cycles = 5;
    }

    /**
     * Move value in memory to register
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param value_in_memory is the value to move into to
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mov_r_m(u8 &to, u8 value_in_memory, cyc &cycles) {
        mov(to, value_in_memory);

        cycles = 7;
    }

    /**
     * Move register to memory
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param memory is the memory, which will be mutated
     * @param address is the address of value in memory to move value to
     * @param new_value is the value to move into to
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mov_m_r(EmulatorMemory<u16, u8> &memory, u16 address, u8 new_value, cyc &cycles) {
        u8 value_in_memory = memory.read(address);

        mov(value_in_memory, new_value);

        memory.write(address, new_value);

        cycles = 7;
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

            mov_r_r(reg1, reg2, cycles);
            CHECK_EQ(0, reg1);

            reg2 = 0xa;
            mov_r_r(reg1, reg2, cycles);
            CHECK_EQ(0xa, reg1);

            reg2 = 0xff;
            mov_r_r(reg1, reg2, cycles);
            CHECK_EQ(0xff, reg1);
        }

        SUBCASE("should load the memory with value") {
            EmulatorMemory<u16, u8> memory;
            memory.add({0x10});
            u16 address = 0x0000;
            u8 value = 0;

            mov_m_r(memory, address, value, cycles);
            CHECK_EQ(0, memory.read(address));

            value = 0xa;
            mov_m_r(memory, address, value, cycles);
            CHECK_EQ(0xa, memory.read(address));

            value = 0xff;
            mov_m_r(memory, address, value, cycles);
            CHECK_EQ(0xff, memory.read(address));
        }

        SUBCASE("should use 5 cycles if memory is not involved") {
            cycles = 0;
            u8 reg1 = 0;
            u8 reg2 = 0x11;

            mov_r_r(reg1, reg2, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 7 cycles if memory is involved") {
            cycles = 0;
            EmulatorMemory<u16, u8> memory;
            memory.add({0x10});
            u16 address = 0x0000;
            u8 value = 0x20;

            mov_m_r(memory, address, value, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
