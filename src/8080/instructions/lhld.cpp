#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Load H and L direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param l_reg is the L register, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param memory is the memory
     * @param args contains the argument with the address to the wanted value in memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lhld(std::uint8_t &l_reg, std::uint8_t &h_reg, const emu::cpu8080::EmulatorMemory &memory,
              const NextWord &args, unsigned long &cycles) {
        l_reg = memory[emu::util::byte::to_u16(args.sarg, args.farg)];
        h_reg = memory[emu::util::byte::to_u16(args.sarg, args.farg) + 1];

        cycles = 16;
    }

    void print_lhld(std::ostream& ostream, const NextWord &args) {
        ostream << "LHLD "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
