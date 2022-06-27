#include <iostream>
#include "doctest.h"
#include "z80/next_word.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::string::hexify_wo_0x;

    /**
     * Call
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 5</li>
     *   <li>States: 17</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, unsigned long &cycles) {
        execute_call(pc, sp, memory, args);

        cycles = 17;
    }

    void print_call(std::ostream &ostream, const NextWord &args) {
        ostream << "CALL "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: CALL") {
        unsigned long cycles = 0;
        EmulatorMemory memory;
        memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA});
        NextWord args = {.farg = 0x2, .sarg = 0x0};

        SUBCASE("should push current PC on the stack and change PC to the address in args") {
            u16 pc = 0x100f;
            u16 sp = 0x2;

            call(pc, sp, memory, args, cycles);

            CHECK_EQ(0x0002, pc);
            CHECK_EQ(0x0f, memory[0]);
            CHECK_EQ(0x10, memory[1]);
        }

        SUBCASE("should use 17 cycles") {
            cycles = 0;

            u16 pc = 0;
            u16 sp = 0x2;

            call(pc, sp, memory, args, cycles);

            CHECK_EQ(17, cycles);
        }
    }
}
