#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <iostream>
#include <string>
#include <vector>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;
    using emu::memory::NextWord;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify_wo_0x;

    /**
     * Store H and L direct
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 16</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param h_reg is the H register
     * @param l_reg is the L register
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void shld(u8 l_reg, u8 h_reg, EmulatorMemory &memory, const NextWord &args, cyc &cycles) {
        const u16 l_address = to_u16(args.sarg, args.farg);
        const u16 h_address = l_address + 1;

        memory.write(l_address, l_reg);
        memory.write(h_address, h_reg);

        cycles = 16;
    }

    void print_shld(std::ostream &ostream, const NextWord &args) {
        ostream << "SHLD "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: SHLD") {
        cyc cycles = 0;
        u8 l_reg = 0x22;
        u8 h_reg = 0x11;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0xff, 0xaa, 0xbb, 0xcc, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x2, .sarg = 0x0};

        SUBCASE("should load memory with H and L registers") {
            shld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(l_reg, memory.read(0x02));
            CHECK_EQ(h_reg, memory.read(0x03));
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;

            shld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(16, cycles);
        }
    }
}
