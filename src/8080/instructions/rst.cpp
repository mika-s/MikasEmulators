#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    void rst(std::uint16_t &pc, std::uint16_t new_pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {

        execute_call(pc, sp, memory, emu::util::byte::first_byte(new_pc), emu::util::byte::second_byte(new_pc));

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
    void rst_0(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_1(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_2(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_3(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_4(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_5(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_6(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
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
    void rst_7(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, unsigned long &cycles) {
        rst(pc, 0x38, sp, memory, cycles);
    }

    void print_rst(int number) {
        std::cout << "RST " << number;
    }
}
