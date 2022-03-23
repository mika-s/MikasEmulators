#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "8080/flags.h"

namespace emu::cpu8080 {
    /**
     * Pop
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param reg1 is the first register to pop to, which will be mutated
     * @param reg2 is the second register to pop to, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop(std::uint8_t &reg1, std::uint8_t &reg2, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        reg2 = memory[sp++];
        reg1 = memory[sp++];

        cycles = 10;
    }

    /**
     * Pop
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param flag_reg is the flag register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void pop_psw(Flags &flag_reg, std::uint8_t &acc_reg, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        flag_reg.from_uint8_t(memory[sp++]);
        acc_reg = memory[sp++];

        cycles = 10;
    }

    void print_pop(const std::string &reg) {
        std::cout << "POP " << reg;
    }
}
