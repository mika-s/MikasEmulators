#include <cstdint>
#include <iostream>
#include "byte_util.h"
#include "emulator_memory.h"
#include "next_word.h"
#include "string_util.h"

namespace emu::cpu8080 {
    /**
     * Load accumulator direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 13</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param memory is the memory
     * @param args contains the argument with the address
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lda(std::uint8_t &acc_reg, const emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
            unsigned long &cycles) {
        acc_reg = memory[emu::util::byte::to_uint16_t(args.sarg, args.farg)];

        cycles = 13;
    }

    void print_lda(const NextWord &args) {
        std::cout << "LDA "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
