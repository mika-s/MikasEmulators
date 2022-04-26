#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/flags.h"
#include "8080/next_word.h"
#include "8080/instructions/instruction_util.h"
#include "crosscutting/byte_util.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Call if carry
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
    void cc(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, const NextWord &args,
            const Flags &flag_reg, unsigned long &cycles) {
        cycles = 0;

        if (flag_reg.is_carry_flag_set()) {
            execute_call(pc, sp, memory, args);
            cycles += 6;
        }

        cycles += 11;
    }

    void print_cc(std::ostream &ostream, const NextWord &args) {
        ostream << "CC "
                << emu::util::string::hexify_wo_0x(args.sarg)
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: CC") {
        unsigned long cycles = 0;

        SUBCASE("should push current PC on the stack and change PC to the address in args when the carry flag is set") {
            std::uint16_t pc = 0x100f;
            std::uint16_t sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            cc(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(emu::util::byte::to_u16(args.sarg, args.farg), pc);
            CHECK_EQ(0x0f, memory[0]);
            CHECK_EQ(0x10, memory[1]);
        }

        SUBCASE("should not do anything when carry flag is unset") {
            std::uint16_t pc = 0x100f;
            std::uint16_t sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            cc(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(0x100f, pc);
            CHECK_EQ(0x00, memory[0]);
            CHECK_EQ(0x01, memory[1]);
        }

        SUBCASE("should use 11 cycles when not called") {
            cycles = 0;
            std::uint16_t pc = 0;
            std::uint16_t sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.clear_carry_flag();

            cc(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(11, cycles);
        }

        SUBCASE("should use 17 cycles when called") {
            cycles = 0;
            std::uint16_t pc = 0;
            std::uint16_t sp = 0x2;
            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xa});
            NextWord args = {.farg = 0x2, .sarg = 0x0};
            Flags flag_reg;
            flag_reg.set_carry_flag();

            cc(pc, sp, memory, args, flag_reg, cycles);

            CHECK_EQ(17, cycles);
        }
    }
}
