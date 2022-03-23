#include <cstdint>
#include <iostream>
#include "8080/instructions/instructions.h"

namespace emu::cpu8080 {
    /**
     * Move
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param value is the value to move into to_reg
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mov(std::uint8_t &to, std::uint8_t value, unsigned long &cycles) {
        mov(to, value, cycles, false);
    }

    /**
     * Move
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 5 or 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param to is the register or memory location to move value to
     * @param value is the value to move into to_reg
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void mov(std::uint8_t &to, std::uint8_t value, unsigned long &cycles, bool is_memory_involved) {
        to = value;

        cycles = 5;

        if (is_memory_involved) {
            cycles += 2;
        }
    }

    void print_mov(const std::string &reg1, const std::string &reg2) {
        std::cout << "MOV " << reg1 << "," << reg2;
    }
}
