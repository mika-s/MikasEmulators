#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/emulator_memory.h"
#include "8080/flags.h"

namespace emu::cpu8080 {
    /**
     * Push
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg1 is the first register to place in memory
     * @param reg1 is the second register to place in memory
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push(std::uint8_t reg1, std::uint8_t reg2, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory,
              unsigned long &cycles) {
        memory[--sp] = reg1;
        memory[--sp] = reg2;

        cycles = 11;
    }

    /**
     * Push
     * <ul>
     *   <li>Size: 1</li>
     *   <li>Cycles: 3</li>
     *   <li>States: 11</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param flag_reg is the flag register
     * @param acc_reg is the accumulator register
     * @param sp is the stack pointer, which will be mutated
     * @param memory is the memory, which will be mutated
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void push_psw(const Flags &flag_reg, std::uint8_t &acc_reg, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory,
                  unsigned long &cycles) {
        memory[--sp] = acc_reg;
        memory[--sp] = flag_reg.to_uint8_t();

        cycles = 11;
    }

    void print_push(std::ostream &ostream, const std::string &reg) {
        ostream << "PUSH "
                << reg;
    }

    TEST_CASE("8080: PUSH") {
        unsigned long cycles = 0;

        SUBCASE("should push registers onto the stack") {
            std::uint8_t reg1 = 0xaa;
            std::uint8_t reg2 = 0xbb;
            std::uint16_t sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(reg1, memory[0x2]);
            CHECK_EQ(reg2, memory[0x1]);
            CHECK_EQ(0x01, sp);
        }

        SUBCASE("should push PSW onto the stack") {
            Flags flag_reg;
            flag_reg.set_carry_flag();
            flag_reg.set_zero_flag();
            flag_reg.set_sign_flag();
            flag_reg.set_parity_flag();
            flag_reg.set_aux_carry_flag();
            std::uint8_t acc_reg = 0xbb;
            std::uint16_t sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push_psw(flag_reg, acc_reg, sp, memory, cycles);

            CHECK_EQ(acc_reg, memory[0x2]);
            CHECK_EQ(flag_reg.to_uint8_t(), memory[0x1]);
            CHECK_EQ(0x01, sp);
        }

        SUBCASE("should use 11 cycles") {
            cycles = 0;

            std::uint8_t reg1 = 0;
            std::uint8_t reg2 = 0;
            std::uint16_t sp = 0x03;

            EmulatorMemory memory;
            memory.add(std::vector<std::uint8_t>{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

            push(reg1, reg2, sp, memory, cycles);

            CHECK_EQ(11, cycles);
        }
    }
}
