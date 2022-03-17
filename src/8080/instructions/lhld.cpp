#include <cstdint>
#include <iostream>
#include <string_util.h>
#include "byte_util.h"
#include "emulator_memory.h"
#include "next_word.h"

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
        l_reg = memory[emu::util::byte::to_uint16_t(args.sarg, args.farg)];
        h_reg = memory[emu::util::byte::to_uint16_t(args.sarg, args.farg) + 1];

        cycles = 16;
    }

    void print_lhld(const NextWord &args) {
        std::cout << "LHLD "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
