#pragma once

#include "usings.h"
#include <iosfwd>

namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::synacor {

using emu::memory::EmulatorMemory;

class Disassembler {
public:
    Disassembler(EmulatorMemory<Address, Data>& memory, std::ostream& iostream);

    void disassemble();

private:
    EmulatorMemory<Address, Data>& m_memory;
    std::size_t m_memory_size;
    Address m_pc;
    Data m_opcode { 0 };
    std::ostream& m_ostream;

    void print_next_instruction();

    Data get_next_value();
};
}
