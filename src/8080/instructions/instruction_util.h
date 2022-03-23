#ifndef MIKA_EMULATORS_8080_INSTRUCTIONS_INSTRUCTION_UTIL_H
#define MIKA_EMULATORS_8080_INSTRUCTIONS_INSTRUCTION_UTIL_H

#include <vector>
#include "8080/emulator_memory.h"
#include "8080/flags.h"
#include "8080/next_word.h"

#define LOW_BIT 0
#define HIGH_BIT 7

namespace emu::cpu8080 {
    void add_to_register(std::uint8_t &acc_reg, std::uint8_t value, bool cf, Flags &flag_reg);

    void sub_from_register(std::uint8_t &acc_reg, std::uint8_t value, bool cf, Flags &flag_reg);

    void execute_call(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, const NextWord &args);

    void execute_call(std::uint16_t &pc, std::uint16_t &sp, emu::cpu8080::EmulatorMemory &memory, std::uint8_t farg, std::uint8_t sarg);

    void execute_return(std::uint16_t &pc, std::uint16_t &sp, const emu::cpu8080::EmulatorMemory &memory);
}

#endif //MIKA_EMULATORS_8080_INSTRUCTIONS_INSTRUCTION_UTIL_H
