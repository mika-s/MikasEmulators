#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    /**
     * Load accumulator indirect
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param reg1 contains the MSB of the address to load
     * @param reg2 contains the LSB of the address to load
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ldax(std::uint8_t &acc_reg, std::uint8_t reg1, std::uint8_t reg2, const emu::cpu8080::EmulatorMemory &memory,
            unsigned long &cycles) {
        acc_reg = memory[emu::util::byte::to_uint16_t(reg1, reg2)];

        cycles = 7;
    }

    void print_ldax(const std::string &reg) {
        std::cout << "LDAX " << reg;
    }
}
