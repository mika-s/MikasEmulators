#include <iostream>
#include "doctest.h"
#include "8080/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::cpu8080 {

    using emu::util::byte::to_u16;

    /**
     * Load accumulator indirect
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param acc_reg is the accumulator register, which will be mutated
     * @param reg1 contains the MSB of the address to load
     * @param reg2 contains the LSB of the address to load
     * @param memory is the memory
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void ldax(u8 &acc_reg, u8 reg1, u8 reg2, const EmulatorMemory &memory, unsigned long &cycles) {
        acc_reg = memory[to_u16(reg1, reg2)];

        cycles = 7;
    }

    void print_ldax(std::ostream &ostream, const std::string &reg) {
        ostream << "LDAX "
                << reg;
    }

    TEST_CASE("8080: LDAX") {
        unsigned long cycles = 0;
        u8 acc_reg = 0xe;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0xfd, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
        u8 reg1 = 0;
        u8 reg2 = 0x04;

        SUBCASE("should load the accumulator from memory using address in args") {
            ldax(acc_reg, reg1, reg2, memory, cycles);

            CHECK_EQ(memory[0x04], acc_reg);
        }

        SUBCASE("should use 7 cycles") {
            cycles = 0;

            ldax(acc_reg, reg1, reg2, memory, cycles);

            CHECK_EQ(7, cycles);
        }
    }
}
