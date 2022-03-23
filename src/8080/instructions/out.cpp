#include <cstdint>
#include <vector>
#include <iostream>
#include "8080/next_byte.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Output to port
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param args contains the argument with the output port number
     * @param io is the IO addresses, which might be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void out(std::uint8_t acc_reg, const NextByte &args, std::vector<std::uint8_t> &io, unsigned long &cycles) {
        io[args.farg] = acc_reg;

        cycles = 10;
    }

    void print_out(const NextByte &args) {
        std::cout << "OUT "
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
