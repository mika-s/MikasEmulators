#ifndef MIKA_EMULATORS_CHIPS_8080_DISASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_8080_DISASSEMBLER_H

#include <string>
#include "crosscutting/typedefs.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;
    using emu::memory::NextByte;
    using emu::memory::NextWord;

    class Disassembler8080 {
    public:
        Disassembler8080(EmulatorMemory &memory, std::ostream& iostream);

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

#endif //MIKA_EMULATORS_CHIPS_8080_DISASSEMBLER_H
