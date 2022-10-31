#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "doctest.h"
#include <iostream>

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;
    using emu::util::byte::to_u16;

    /**
     * Store accumulator indirect
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register
     * @param reg1 is the first register in the register pair
     * @param reg1 is the first register in the register pair
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void stax(u8 acc_reg, u8 reg1, u8 reg2, EmulatorMemory &memory, cyc &cycles) {
        const u16 address = to_u16(reg1, reg2);

        memory.write(address, acc_reg);

        cycles = 7;
    }

    void print_stax(std::ostream &ostream, const std::string &reg) {
        ostream << "STAX "
                << reg;
    }

    TEST_CASE("8080: STAX") {
        cyc cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        u8 acc_reg = 0;
        u8 reg1 = 0x0;
        u8 reg2 = 0x3;

        SUBCASE("should store the accumulator in memory at the given address") {
            stax(acc_reg, reg1, reg2, memory, cycles);

            CHECK_EQ(acc_reg, memory.read(0x3));
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;

            stax(acc_reg, reg1, reg2, memory, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
