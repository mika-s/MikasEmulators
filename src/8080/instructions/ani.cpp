#include <cstdint>
#include <iostream>
#include "flags.h"
#include "next_byte.h"
#include "string_util.h"

namespace emu::cpu8080 {
    /**
     * And immediate with accumulator
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
    void ani(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        std::uint8_t previous = acc_reg;
        acc_reg &= args.farg;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by ANI, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but apparently has to be modified anyway. This is explained here:
        // https://demin.ws/blog/english/2012/12/24/my-i8080-collection/
        // The 8080/8085 Assembly Language Programming Manual also mentions auxiliary carry being modified by ANI.

        if (((previous | args.farg) & 0x08) != 0) {
            flag_reg.set_aux_carry_flag();
        } else {
            flag_reg.clear_aux_carry_flag();
        }

        cycles = 7;
    }

    void print_ani(const NextByte &args) {
        std::cout << "ANI "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
