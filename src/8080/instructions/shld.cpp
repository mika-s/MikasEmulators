#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Store H and L direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register
     * @param l_reg is the L register
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void shld(std::uint8_t l_reg, std::uint8_t h_reg, emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
            unsigned long &cycles) {
        const std::uint16_t l_address = emu::util::byte::to_u16(args.sarg, args.farg);
        const std::uint16_t h_address = l_address + 1;

        memory[l_address] = l_reg;
        memory[h_address] = h_reg;

        cycles = 16;
    }

    void print_shld(std::ostream& ostream, const NextWord &args) {
        ostream << "SHLD "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
