#include <iostream>
#include "doctest.h"
#include "8080/emulator_memory.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080 {

    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Store accumulator direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 13</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param value is the memory, which will be mutated
     * @param args contains the argument with the address in memory to store the accumulator register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void sta(u8 &acc_reg, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        const u16 address = to_u16(args.sarg, args.farg);

        memory[address] = acc_reg;

        cycles = 13;
    }

    void print_sta(std::ostream &ostream, const NextWord &args) {
        ostream << "STA "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: STA") {
        unsigned long cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        u8 acc_reg = 0x45;
        NextWord args = {.farg = 0x3, .sarg = 0x0};

        SUBCASE("should store the accumulator in memory at the given address") {
            sta(acc_reg, memory, args, cycles);

            CHECK_EQ(acc_reg, memory[0x3]);
        }

        SUBCASE("should use 13 cycles") {
            cycles = 0;

            sta(acc_reg, memory, args, cycles);

            CHECK_EQ(13, cycles);
        }
    }
}
