#ifndef MIKA_EMULATORS_8080_DISASSEMBLER_H
#define MIKA_EMULATORS_8080_DISASSEMBLER_H

#include <cstdint>
#include <string>
#include "8080/next_byte.h"
#include "8080/next_word.h"
#include "8080/emulator_memory.h"

namespace emu::cpu8080 {

    class Disassembler8080 {
    public:
        Disassembler8080(emu::cpu8080::EmulatorMemory &memory, std::ostream& iostream);

        void disassemble();

    private:
        emu::cpu8080::EmulatorMemory &m_memory;
        std::size_t m_memory_size;
        std::uint16_t m_pc;
        std::uint8_t m_opcode;
        std::ostream &m_ostream;

        void print_next_instruction();

        NextByte get_next_byte();

        NextWord get_next_word();
    };
}

#endif //MIKA_EMULATORS_8080_DISASSEMBLER_H
