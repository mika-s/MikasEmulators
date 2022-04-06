#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Jump if positive
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
    void jp(std::uint16_t &pc, const NextWord &args, const Flags &flag_reg, unsigned long &cycles) {
        if (!flag_reg.is_sign_flag_set()) {
            pc = emu::util::byte::to_u16(args.sarg, args.farg);
        }

        cycles = 10;
    }

    void print_jp(std::ostream& ostream, const NextWord &args) {
        ostream << "JP "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
