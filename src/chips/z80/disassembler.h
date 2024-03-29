#pragma once

#include "crosscutting/memory/next_byte.h"
#include "crosscutting/memory/next_word.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <iosfwd>
#include <string>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::z80 {

using emu::memory::EmulatorMemory;
using emu::memory::NextByte;
using emu::memory::NextWord;

class Disassembler {
public:
    Disassembler(EmulatorMemory<u16, u8>& memory, std::ostream& iostream);

    void disassemble();

private:
    EmulatorMemory<u16, u8>& m_memory;
    std::size_t m_memory_size;
    u16 m_pc;
    u8 m_opcode;
    std::ostream& m_ostream;

    void print_next_instruction();

    void print_next_bits_instruction(u8 bits_opcode);

    void print_next_ixy_instruction(u8 ixy_opcode, std::string const& ixy_reg);

    void print_next_ixy_bits_instruction(NextWord args, std::string const& ixy_reg);

    void print_next_extd_instruction(u8 extd_opcode);

    NextByte get_next_byte();

    NextWord get_next_word();
};
}
