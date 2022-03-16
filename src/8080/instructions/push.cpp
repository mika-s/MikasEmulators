#include <cstdint>
#include <iostream>
#include "emulator_memory.h"
#include "flags.h"

namespace emu::cpu8080 {
    /**
     * Push
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to place in memory
     * @param reg1 is the second register to place in memory
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push(std::uint8_t reg1, std::uint8_t reg2, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        memory[--sp] = reg1;
        memory[--sp] = reg2;

        cycles = 11;
    }

    /**
     * Push
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
    void push_psw(const Flags &flag_reg, std::uint8_t &acc_reg, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        memory[--sp] = acc_reg;
        memory[--sp] = flag_reg.to_uint8_t();

        cycles = 11;
    }

    void print_push(const std::string &reg) {
        std::cout << "PUSH " << reg;
    }
}
