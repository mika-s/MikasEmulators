#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Jump if parity even
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param args contains the argument with the address to jump to
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void jpe(std::uint16_t &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles) {
        if (flag_reg.is_parity_flag_set()) {
            pc = emu::util::byte::to_uint16_t(args.sarg, args.farg);
        }

        cycles = 10;
    }

    void print_jpe(const NextWord &args) {
        std::cout << "JPE "
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
