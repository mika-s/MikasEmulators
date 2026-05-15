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
    Disassembler(EmulatorMemory<Address, Data>& memory, std::ostream& ostream);

    void disassemble();

private:
    EmulatorMemory<Address, Data>& m_memory;
    Address m_pc;
    std::ostream& m_ostream;

    void print_next_instruction();

    auto get_next_value() -> Data;

    static auto find_argument(Data raw_opcode) -> Address;

    static auto find_opcode(Data raw_opcode) -> Opcode;
};
}
