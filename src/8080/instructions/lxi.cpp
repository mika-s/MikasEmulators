#include <cstdint>
#include <iostream>
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Load register pair immediate
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to load into, which will be mutated
     * @param reg2 is the second register to load into, which will be mutated
     * @param args contains value to load into the registers
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lxi(std::uint8_t &reg1, std::uint8_t &reg2, const NextWord &args, unsigned long &cycles) {
        reg1 = args.sarg;
        reg2 = args.farg;

        cycles = 10;
    }

    /**
     * Load register pair immediate (for SP)
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param sp is the stack pointer, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lxi_sp(std::uint16_t &sp, const NextWord &args, unsigned long &cycles) {
        sp = emu::util::byte::to_uint16_t(args.sarg, args.farg);

        cycles = 10;
    }

    void print_lxi(const std::string &reg, const NextWord &args) {
        std::cout << "LXI " << reg << ","
                  << emu::util::string::hexify_wo_0x(args.sarg)
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
