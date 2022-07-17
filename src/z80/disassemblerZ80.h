#ifndef MIKA_EMULATORS_Z80_DISASSEMBLER_H
#define MIKA_EMULATORS_Z80_DISASSEMBLER_H

#include <string>
#include "z80/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/next_byte.h"
#include "crosscutting/misc/next_word.h"

namespace emu::z80 {

    using emu::misc::NextByte;
    using emu::misc::NextWord;

    class DisassemblerZ80 {
    public:
        DisassemblerZ80(EmulatorMemory &memory, std::ostream& iostream);

        void disassemble();

    private:
        EmulatorMemory &m_memory;
        std::size_t m_memory_size;
        u16 m_pc;
        u8 m_opcode;
        std::ostream &m_ostream;

        void print_next_instruction();

        NextByte get_next_byte();

        NextWord get_next_word();
    };
}

#endif //MIKA_EMULATORS_Z80_DISASSEMBLER_H
