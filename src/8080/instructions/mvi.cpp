#include <cstdint>
#include <iostream>
#include "instructions.h"
#include "next_byte.h"
#include "string_util.h"

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

    void print_mvi(const std::string &reg, const NextByte &args) {
        std::cout << "MVI " << reg << ","
                  << emu::util::string::hexify_wo_0x(args.farg);
    }
}
