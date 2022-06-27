#include <iostream>
#include "doctest.h"
#include "z80/flags.h"
#include "z80/next_word.h"
#include "z80/instructions/instruction_util.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"

namespace emu::z80 {

    using emu::util::string::hexify_wo_0x;

    /**
     * Call if parity odd
     * <ul>
     *   <li>Size: 3</li>
     *   <li>Cycles: 3 or 5</li>
     *   <li>States: 11 or 17</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param pc is the program counter, which will be mutated
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param args contains the argument with the address to call
     * @param flag_reg is the flag register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void cpo(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args, const Flags &flag_reg,
             unsigned long &cycles) {
        cycles = 0;

        if (!flag_reg.is_parity_flag_set()) {
            execute_call(pc, sp, memory, args);
            cycles += 6;
        }

        cycles += 11;
    }

    void print_cpo(std::ostream &ostream, const NextWord &args) {
        ostream << "CPO "
                << hexify_wo_0x(args.sarg)
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("Z80: CPO") {
        unsigned long cycles = 0;

        SUBCASE("should push current PC on the stack and change PC to the address in args when the parity flag is unset") {
            u16 pc = 0x100f;
            u16 sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.clear_parity_flag();

            cpo(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(emu::util::byte::to_u16(args.sarg, args.farg), pc);
            CHECK_EQ(0x0f, memory[0]);
            CHECK_EQ(0x10, memory[1]);
        }

        SUBCASE("should not do anything when the parity flag is set") {
            u16 pc = 0x100f;
            u16 sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.set_parity_flag();

            cpo(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
            CHECK_EQ(0x00, memory[0]);
            CHECK_EQ(0x01, memory[1]);
        }

        SUBCASE("should use 11 cycles when not called") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.set_parity_flag();

            cpo(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }

        SUBCASE("should use 17 cycles when called") {
            cycles = 0;
            u16 pc = 0;
            u16 sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<u8>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.clear_parity_flag();

            cpo(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(17, cycles);
        }
    }
}
