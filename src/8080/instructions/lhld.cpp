#include <iostream>
#include "doctest.h"
#include "8080/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_word.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextWord;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;
    
    /**
     * Load H and L direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param l_reg is the L register, which will be mutated
     * @param h_reg is the H register, which will be mutated
     * @param memory is the memory
     * @param args contains the argument with the address to the wanted value in memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void lhld(u8 &l_reg, u8 &h_reg, const EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        l_reg = memory[to_u16(args.sarg, args.farg)];
        h_reg = memory[to_u16(args.sarg, args.farg) + 1];

        cycles = 16;
    }

    void print_lhld(std::ostream &ostream, const NextWord &args) {
        ostream << "LHLD "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: LHLD") {
        unsigned long cycles = 0;
        u8 l_reg = 0xe;
        u8 h_reg = 0x42;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x04, .sarg = 0};

        SUBCASE("should load the accumulator from memory using the address in args") {
            lhld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(memory[0x04], l_reg);
            CHECK_EQ(memory[0x05], h_reg);
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;

            lhld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(16, cycles);
        }
    }
}
