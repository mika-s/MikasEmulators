#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Input from port
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param args contains the argument with the input port number
     * @param io is the IO addresses
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void in(std::uint8_t &acc_reg, const NextByte &args, std::vector<std::uint8_t> io, unsigned long &cycles) {
        acc_reg = io[args.farg];

        cycles = 10;
    }

    void print_in(std::ostream& ostream, const NextByte &args) {
        ostream << "IN "
                << emu::util::string::hexify_wo_0x(args.farg);
    }
}
