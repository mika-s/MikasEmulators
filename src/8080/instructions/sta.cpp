#include <cstdint>
#include <iostream>
#include "8080/emulator_memory.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Store accumulator direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 13</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the memory, which will be mutated
     * @param args contains the argument with the address in memory to store the accumulator register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sta(std::uint8_t &acc_reg, emu::cpu8080::EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        const std::uint16_t address = emu::util::byte::to_u16(args.sarg, args.farg);

        memory[address] = acc_reg;

        cycles = 13;
    }

    void print_sta(std::ostream& ostream, const NextWord &args) {
        ostream << "STA "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
