#include <cstdint>
#include <iostream>
#include "8080/next_word.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Call
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 17</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void call(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
            unsigned long &cycles) {
        execute_call(pc, sp, memory, args);

        cycles = 17;
    }

    void print_call(const NextWord &args) {
        std::cout << "CALL "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
