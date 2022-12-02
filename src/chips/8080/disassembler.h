#ifndef MIKA_EMULATORS_CHIPS_8080_DISASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_8080_DISASSEMBLER_H

#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <iosfwd>

namespace emu::memory {
    template<class A, class D>
    class EmulatorMemory;
}

namespace emu::i8080 {

    using emu::memory::EmulatorMemory;
    using emu::memory::NextByte;
    using emu::memory::NextWord;

    class Disassembler {
    public:
        Disassembler(EmulatorMemory<u16, u8> &memory, std::ostream &iostream);

        void disassemble();

    private:
        EmulatorMemory<u16, u8> &m_memory;
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
