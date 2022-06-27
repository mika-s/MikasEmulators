#ifndef MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H
#define MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H

#include <vector>
#include "z80/emulator_memory.h"
#include "z80/flags.h"
#include "z80/next_word.h"
#include "crosscutting/typedefs.h"

#define LOW_BIT 0
#define HIGH_BIT 7

namespace emu::z80 {

    void add_to_register(u8 &acc_reg, u8 value, bool cf, Flags &flag_reg);

    void sub_from_register(u8 &acc_reg, u8 value, bool cf, Flags &flag_reg);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, const NextWord &args);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory &memory, u8 farg, u8 sarg);

    void execute_return(u16 &pc, u16 &sp, const EmulatorMemory &memory);
}

#endif //MIKA_EMULATORS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H
