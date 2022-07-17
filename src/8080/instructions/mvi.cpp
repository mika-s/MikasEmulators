#include <iostream>
#include "doctest.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/util/string_util.h"

namespace emu::i8080 {

    using emu::misc::NextByte;
    using emu::util::string::hexify_wo_0x;

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
    void mvi(u8 &reg, const NextByte &args, unsigned long &cycles, bool is_memory_involved) {
        reg = args.farg;

        cycles = 7;

        if (is_memory_involved) {
            cycles += 3;
        }
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
     */
    void mvi(u8 &reg, const NextByte &args, unsigned long &cycles) {
        mvi(reg, args, cycles, false);
    }

    void print_mvi(std::ostream &ostream, const std::string &reg, const NextByte &args) {
        ostream << "MVI "
                << reg << ","
                << hexify_wo_0x(args.farg);
    }

    TEST_CASE("8080: MVI") {
        unsigned long cycles = 0;

        SUBCASE("should load registers with value") {
            u8 reg = 0;
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
            u8 memory_location = 0;
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
            u8 reg = 0;
            NextByte args = {.farg = 0x11};

            mvi(reg, args, cycles);

            CHECK_EQ(7, cycles);
        }

        SUBCASE("should use 10 cycles if memory is involved") {
            cycles = 0;
            u8 memory_location = 0;
            NextByte args = {.farg = 0x21};

            mvi(memory_location, args, cycles, true);

            CHECK_EQ(10, cycles);
        }
    }
}
