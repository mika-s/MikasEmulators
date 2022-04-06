#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Add immediate to accumulator with carry
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
    void aci(std::uint8_t &acc_reg, NextByte args, Flags &flag_reg, unsigned long &cycles) {
        const std::uint8_t previous = acc_reg;
        const std::uint8_t carry = flag_reg.is_carry_flag_set() ? 1 : 0;
        acc_reg += args.farg + carry;

        flag_reg.handle_aux_carry_flag(previous, args.farg, carry);
        flag_reg.handle_carry_flag(previous, args.farg + carry);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        cycles = 7;
    }

    void print_aci(std::ostream& ostream, const NextByte &args) {
        ostream << "ACI "
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
