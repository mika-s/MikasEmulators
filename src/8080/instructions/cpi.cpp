#include <cstdint>
#include <string_util.h>
#include <iostream>
#include "flags.h"
#include "next_byte.h"

namespace emu::cpu8080 {
    /**
     * Compare immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cpi(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        const std::uint16_t new_acc_reg = acc_reg - args.farg;

        flag_reg.handle_borrow_flag(acc_reg, args.farg);
        flag_reg.handle_zero_flag(new_acc_reg);
        flag_reg.handle_parity_flag(new_acc_reg);
        flag_reg.handle_sign_flag(new_acc_reg);
        flag_reg.handle_aux_borrow_flag(acc_reg, args.farg, false);

        cycles = 7;
    }

    void print_cpi(const NextByte &args) {
        std::cout << "CPI "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
