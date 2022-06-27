#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"

namespace emu::i8080 {
    /**
     * Return if positive
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 1 or 3</li>
     *   <li>States: 5 or 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void rp(u16 &pc, u16 &sp, const EmulatorMemory &memory, const Flags &flag_reg, unsigned long &cycles) {
        cycles = 0;

        if (!flag_reg.is_sign_flag_set()) {
            execute_return(pc, sp, memory);

            cycles += 6;
        }

        cycles += 5;
    }

    void print_rp(std::ostream &ostream) {
        ostream << "RP";
    }

    TEST_CASE("8080: RP") {
        unsigned long cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0xab, 0x01, 0x02, 0x03, 0x04, 0x05});

        SUBCASE("should pop PC off the stack when the sign flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.clear_sign_flag();

            rp(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(emu::util::byte::to_u16(memory[1], memory[0]), pc);
        }

        SUBCASE("should not pop PC off the stack when the sign flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            rp(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
        }

        SUBCASE("should use 5 cycles when not returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;
            flag_reg.set_sign_flag();

            rp(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(5, cycles);
        }

        SUBCASE("should use 11 cycles when returning") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0;
            Flags flag_reg;

            rp(pc, sp, memory, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
