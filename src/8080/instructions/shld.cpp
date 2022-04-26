#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/emulator_memory.h"
#include "8080/next_word.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
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
    void shld(std::uint8_t l_reg, std::uint8_t h_reg, emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
              unsigned long &cycles) {
        const std::uint16_t l_address = emu::util::byte::to_u16(args.sarg, args.farg);
        const std::uint16_t h_address = l_address + 1;

        memory[l_address] = l_reg;
        memory[h_address] = h_reg;

        cycles = 16;
    }

    void print_shld(std::ostream &ostream, const NextWord &args) {
        ostream << "SHLD "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: SHLD") {
        unsigned long cycles = 0;
        std::uint8_t l_reg = 0x22;
        std::uint8_t h_reg = 0x11;
        EmulatorMemory memory;
        memory.add(std::vector<std::uint8_t>{0x00, 0xff, 0xaa, 0xbb, 0xcc, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        NextWord args = {.farg = 0x2, .sarg = 0x0};

        SUBCASE("should load memory with H and L registers") {
            shld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(l_reg, memory[0x02]);
            CHECK_EQ(h_reg, memory[0x03]);
        }

        SUBCASE("should use 16 cycles") {
            cycles = 0;

            shld(l_reg, h_reg, memory, args, cycles);

            CHECK_EQ(16, cycles);
        }
    }
}
