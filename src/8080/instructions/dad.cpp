#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "crosscutting/byte_util.h"

namespace emu::cpu8080 {
    /**
     * Double register add
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: carry</li>
     * </ul>
     *
     * @param h_reg is the H register, which will be mutated
     * @param l_reg is the L register, which will be mutated
     * @param value_to_add contains the argument that should be added to HL
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void dad(std::uint8_t &h_reg, std::uint8_t &l_reg, std::uint16_t value_to_add, Flags &flag_reg, unsigned long &cycles) {
        const std::uint16_t previous = emu::util::byte::to_u16(h_reg, l_reg);
        const std::uint16_t next = previous + value_to_add;
        h_reg = emu::util::byte::second_byte(next);
        l_reg = emu::util::byte::first_byte(next);

        flag_reg.handle_carry_flag_dad(previous, value_to_add);

        cycles = 10;
    }

    void print_dad(std::ostream& ostream, const std::string &reg) {
        ostream << "DAD "
                << reg;
    }
}
