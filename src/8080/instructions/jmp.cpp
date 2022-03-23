#include <cstdint>
#include <iostream>
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Jump
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the address to jump to
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jmp(std::uint16_t &pc, const NextWord &args, unsigned long &cycles) {
        pc = emu::util::byte::to_uint16_t(args.sarg, args.farg);

        cycles = 10;
    }

    void print_jmp(const NextWord &args) {
        std::cout << "JMP "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
