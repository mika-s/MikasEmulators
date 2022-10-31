#ifndef MIKA_EMULATORS_CHIPS_Z80_DISASSEMBLER_H
#define MIKA_EMULATORS_CHIPS_Z80_DISASSEMBLER_H

#include <string>
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"

namespace emu::z80 {

    using emu::memory::EmulatorMemory;
    using emu::memory::NextByte;
    using emu::memory::NextWord;

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

        void print_next_bits_instruction(u8 bits_opcode);

        void print_next_ixy_instruction(u8 ixy_opcode, const std::string &ixy_reg);

        void print_next_ixy_bits_instruction(NextWord args, const std::string &ixy_reg);

        void print_next_extd_instruction(u8 extd_opcode);

        NextByte get_next_byte();

        NextWord get_next_word();
    };
}

#endif //MIKA_EMULATORS_CHIPS_Z80_DISASSEMBLER_H
