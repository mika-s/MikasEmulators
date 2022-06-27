#ifndef MIKA_EMULATORS_Z80_DISASSEMBLER_H
#define MIKA_EMULATORS_Z80_DISASSEMBLER_H

#include <string>
#include "z80/next_byte.h"
#include "z80/next_word.h"
#include "z80/emulator_memory.h"
#include "crosscutting/typedefs.h"

namespace emu::z80 {

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
