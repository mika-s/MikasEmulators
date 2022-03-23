#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Exclusive or immediate with accumulator
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void xri(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        acc_reg ^= args.farg;

        flag_reg.clear_carry_flag();
        flag_reg.handle_zero_flag(acc_reg);
        flag_reg.handle_sign_flag(acc_reg);
        flag_reg.handle_parity_flag(acc_reg);

        // The auxiliary carry is not supposed to be affected by XRI, according to Intel 8080 Assembly Language
        // Programming Manual (Rev B), but should be reset according to the 8080/8085 Assembly Language Programming
        // Manual. It is reset in this emulator.

        flag_reg.clear_aux_carry_flag();

        cycles = 7;
    }

    void print_xri(const NextByte &args) {
        std::cout << "XRI "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
