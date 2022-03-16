#include <cstdint>
#include <iostream>
#include "flags.h"
#include "next_byte.h"
#include "string_util.h"

namespace emu::cpu8080 {
    /**
     * Subtract immediate
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sui(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        const std::uint8_t previous = acc_reg;
        acc_reg -= args.farg;

        flag_reg.handle_aux_borrow_flag(previous, args.farg, false);
        flag_reg.handle_borrow_flag(previous, args.farg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        cycles = 7;
    }

    void print_sui(const NextByte &args) {
        std::cout << "SUI "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
