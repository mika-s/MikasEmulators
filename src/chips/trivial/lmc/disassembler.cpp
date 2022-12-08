#include "disassembler.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "opcode.h"
#include "usings.h"
#include <iostream>

namespace emu::lmc {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::string::hexify_wo_0x;

Disassembler::Disassembler(EmulatorMemory<Address, Data>& memory, std::ostream& ostream)
    : m_memory(memory)
    , m_pc(0)
    , m_ostream(ostream)
{
}

void Disassembler::disassemble()
{
    while (m_pc < Address(99)) {
        print_next_instruction();
    }
}

void Disassembler::print_next_instruction()
{
    m_ostream << m_pc << "\t\t";

    Data raw_opcode = get_next_value();
    const Opcode opcode = find_opcode(raw_opcode);

    switch (opcode) {
    case Opcode::ADD:
        print_add(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::SUB:
        print_sub(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::STA:
        print_sta(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::LDA:
        print_lda(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::BRA:
        print_bra(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::BRZ:
        print_brz(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::BRP:
        print_brp(m_ostream, find_argument(raw_opcode));
        break;
    case Opcode::INP:
        m_ostream << "INP";
        break;
    case Opcode::OUT:
        m_ostream << "OUT";
        break;
    case Opcode::OTC:
        m_ostream << "OTC";
        break;
    case Opcode::HLT:
        if (raw_opcode > Data(0)) {
            m_ostream << "DAT " << raw_opcode;
        } else {
            print_hlt(m_ostream);
        }
        break;
    }

    m_ostream << "\n";
}

Data Disassembler::get_next_value()
{
    return m_memory.read(m_pc++);
}

Address Disassembler::find_argument(Data raw_opcode)
{
    return Address(raw_opcode.underlying());
}

Opcode Disassembler::find_opcode(Data raw_opcode)
{
    if (Data(0) <= raw_opcode && raw_opcode <= Data(99)) {
        return Opcode::HLT;
    } else if (Data(100) <= raw_opcode && raw_opcode <= Data(199)) {
        return Opcode::ADD;
    } else if (Data(200) <= raw_opcode && raw_opcode <= Data(299)) {
        return Opcode::SUB;
    } else if (Data(300) <= raw_opcode && raw_opcode <= Data(399)) {
        return Opcode::STA;
    } else if (Data(500) <= raw_opcode && raw_opcode <= Data(599)) {
        return Opcode::LDA;
    } else if (Data(600) <= raw_opcode && raw_opcode <= Data(699)) {
        return Opcode::BRA;
    } else if (Data(700) <= raw_opcode && raw_opcode <= Data(799)) {
        return Opcode::BRZ;
    } else if (Data(800) <= raw_opcode && raw_opcode <= Data(899)) {
        return Opcode::BRP;
    } else if (raw_opcode == Data(901)) {
        return Opcode::INP;
    } else if (raw_opcode <= Data(902)) {
        return Opcode::OUT;
    } else if (raw_opcode <= Data(922)) {
        return Opcode::OTC;
    } else {
        throw UnrecognizedOpcodeException(raw_opcode.underlying());
    }
}
}
