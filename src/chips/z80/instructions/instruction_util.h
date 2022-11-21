#ifndef MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H
#define MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H

#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"

namespace emu::memory {
    template<class A, class D>
    class EmulatorMemory;
}
namespace emu::z80 {
    class Flags;
}

namespace emu::z80 {

    using emu::memory::EmulatorMemory;
    using emu::memory::NextWord;

    static constexpr unsigned int lsb = 0;
    static constexpr unsigned int msb = 7;

    void add_to_register(u8 &reg, u8 value, bool cf, Flags &flag_reg);

    void add_to_register(u16 &reg, u16 value, bool cf, Flags &flag_reg);

    void sub_from_register(u8 &reg, u8 value, bool cf, Flags &flag_reg);

    void sub_from_register(u16 &reg, u16 value, bool cf, Flags &flag_reg);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory<u16, u8> &memory, const NextWord &args);

    void execute_call(u16 &pc, u16 &sp, EmulatorMemory<u16, u8> &memory, u8 farg, u8 sarg);

    void execute_return(u16 &pc, u16 &sp, const EmulatorMemory<u16, u8> &memory);
}

#endif //MIKA_EMULATORS_CHIPS_Z80_INSTRUCTIONS_INSTRUCTION_UTIL_H
