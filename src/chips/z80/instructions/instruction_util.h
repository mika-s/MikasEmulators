#ifndef MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H
#define MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H

#include <vector>
#include "chips/z80/emulator_memory.h"
#include "chips/z80/flags.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_word.h"

namespace emu::z80 {

    using emu::misc::NextWord;

    static constexpr unsigned int lsb = 0;
    static constexpr unsigned int msb = 7;

    void add_to_register(u8 &reg, u8 value, bool cf, Flags &flag_reg);

    void add_to_register(u16 &reg, u16 value, bool cf, Flags &flag_reg);

    void sub_from_register(u8 &reg, u8 value, bool cf, Flags &flag_reg);

    void sub_from_register(u16 &reg, u16 value, bool cf, Flags &flag_reg);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, u8 farg, u8 sarg);

    void execute_return(u16 &pc, u16 &sp, const EmulatorMemory &memory);
}

#endif //MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H