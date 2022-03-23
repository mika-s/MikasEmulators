#include <cstdint>
#include <iostream>
#include "8080/flags.h"
#include "8080/next_byte.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Subtract immediate with borrow
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1</li>
     *   <li>States: 4</li>
     *   <li>Condition bits affected: carry, auxiliary carry, zero, sign, parity</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the immediate value
     * @param flag_reg is the flag register, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sbi(std::uint8_t &acc_reg, const NextByte &args, Flags &flag_reg, unsigned long &cycles) {
        sub_from_register(acc_reg, args.farg, flag_reg.is_carry_flag_set(), flag_reg);

        cycles = 7;
    }

    void print_sbi(const NextByte &args) {
        std::cout << "SBI "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
