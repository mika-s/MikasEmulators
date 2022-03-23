#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    /**
     * Store accumulator indirect
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param reg1 is the first register in the register pair
     * @param reg1 is the first register in the register pair
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void stax(std::uint8_t acc_reg, std::uint8_t reg1, std::uint8_t reg2, emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        const std::uint16_t address = emu::util::byte::to_uint16_t(reg1, reg2);

        memory[address] = acc_reg;

        cycles = 7;
    }

    void print_stax(const std::string &reg) {
        std::cout << "STAX " << reg;
    }
}
