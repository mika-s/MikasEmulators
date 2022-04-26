#include <cstdint>
#include <iostream>
#include "doctest.h"
#include "8080/next_byte.h"
#include "8080/instructions/instructions.h"
#include "crosscutting/string_util.h"

namespace emu::cpu8080 {
    /**
     * Move immediate
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7 or 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg is the register to load into, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     */
    void mvi(std::uint8_t &reg, const NextByte &args, unsigned long &cycles) {
        mvi(reg, args, cycles, false);
    }

    /**
     * Move immediate
     * <ul>
     *   <li>Size: 2</li>
     *   <li>Cycles: 2</li>
     *   <li>States: 7 or 10</li>
     *   <li>Condition bits affected: none</li>
     * </ul>
     *
     * @param reg is the register to load into, which will be mutated
     * @param args contains value to load into the register
     * @param cycles is the number of cycles variable, which will be mutated
     * @param is_memory_involved is true if memory is involved, either written or read
     */
    void mvi(std::uint8_t &reg, const NextByte &args, unsigned long &cycles, bool is_memory_involved) {
        reg = args.farg;

        cycles = 7;

        if (is_memory_involved) {
            cycles += 3;
        }
    }

    void print_mvi(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "MVI "
                << reg << ","
                << emu::util::string::hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: MVI") {
        unsigned long cycles = 0;

        SUBCASE("should load registers with value") {
            std::uint8_t reg = 0;
            NextByte args = {.farg = 0};

            mvi(reg, args, cycles);
            CHECK_EQ(0, reg);

            args = {.farg = 0xa};
            mvi(reg, args, cycles);
            CHECK_EQ(0xa, reg);

            mvi(reg, args, cycles);
            CHECK_EQ(0xa, reg);

            args = {.farg = 0xff};
            mvi(reg, args, cycles);
            CHECK_EQ(0xff, reg);
        }

        SUBCASE("should load memory with value") {
            std::uint8_t memory_location = 0;
            NextByte args = {.farg = 0};

            mvi(memory_location, args, cycles, true);
            CHECK_EQ(0, memory_location);

            args = {.farg = 0xa};
            mvi(memory_location, args, cycles, true);
            CHECK_EQ(0xa, memory_location);

            mvi(memory_location, args, cycles, true);
            CHECK_EQ(0xa, memory_location);

            args = {.farg = 0xff};
            mvi(memory_location, args, cycles, true);
            CHECK_EQ(0xff, memory_location);
        }

        SUBCASE("should use 7 cycles if memory is not involved") {
            cycles = 0;
            std::uint8_t reg = 0;
            NextByte args = {.farg = 0x11};

            mvi(reg, args, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 10 cycles if memory is involved") {
            cycles = 0;
            std::uint8_t memory_location = 0;
            NextByte args = {.farg = 0x21};

            mvi(memory_location, args, cycles, true);

            CHECK_EQ(10, cycles);
        }
    }
}
