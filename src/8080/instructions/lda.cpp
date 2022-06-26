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
     * Load accumulator direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 4</li>
     *   <li>States: 13</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param memory is the memory
     * @param args contains the argument with the address
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lda(u8 &acc_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        acc_reg = memory[to_u16(args.sarg, args.farg)];

        cycles = 13;
    }

    void print_lda(std::ostream &ostream, const NextWord &args) {
        ostream << "LDA "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: LDA") {
        unsigned long cycles = 0;
        u8 acc_reg = 0xe;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x04, .sarg = 0};

        SUBCASE("should load the accumulator from memory using the address in args") {
            lda(acc_reg, memory, args, cycles);

            CHECK_EQ(memory[0x04], acc_reg);
        }

        SUBCASE("should use 13 cycles") {
            cycles = 0;

            lda(acc_reg, memory, args, cycles);

            CHECK_EQ(13, cycles);
        }
    }
}
