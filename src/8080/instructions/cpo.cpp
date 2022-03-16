#include <cstdint>
#include <iostream>
#include "flags.h"
#include "instruction_util.h"
#include "next_word.h"
#include "string_util.h"

namespace emu::cpu8080 {
    /**
     * Call if parity odd
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3 or 5</li>
     *   <li>States: 11 or 17</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cpo(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
            const Flags &flag_reg, unsigned long &cycles) {
        cycles = 0;

        if (!flag_reg.is_parity_flag_set()) {
            execute_call(pc, sp, memory, args);
            cycles += 6;
        }

        cycles += 11;
    }

    void print_cpo(const NextWord &args) {
        std::cout << "CPO "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
