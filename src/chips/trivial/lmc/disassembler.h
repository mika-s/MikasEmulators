#pragma once

#include "usings.h"
#include <iosfwd>

namespace emu::lmc {
enum class Opcode;
}
namespace emu::memory {
template<class A, class D>
class EmulatorMemory;
}

namespace emu::lmc {

using emu::memory::EmulatorMemory;

class Disassembler {
public:
    Disassembler(EmulatorMemory<Address, Data>& memory, std::ostream& iostream);

    void disassemble();

private:
    EmulatorMemory<Address, Data>& m_memory;
    Address m_pc;
    std::ostream& m_ostream;

    void print_next_instruction();

    Data get_next_value();

    static Address find_argument(Data raw_opcode);

    static Opcode find_opcode(Data raw_opcode);
};
}
