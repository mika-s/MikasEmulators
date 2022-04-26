#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "doctest.h"
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

    void print_in(std::ostream &ostream, const NextByte &args) {
        ostream << "IN "
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: IN") {
        unsigned long cycles = 0;
        std::vector<std::uint8_t> io = {0, 2, 4, 6, 8, 10};
        NextByte args = {.farg = 0x1};
        std::uint8_t acc_reg = 0;

        SUBCASE("should store addressed IO in the accumulator") {
            in(acc_reg, args, io, cycles);

            CHECK_EQ(io[args.farg], acc_reg);
        }

        SUBCASE("should use 10 cycles") {
            cycles = 0;

            in(acc_reg, args, io, cycles);

            CHECK_EQ(10, cycles);
        }
    }
}
