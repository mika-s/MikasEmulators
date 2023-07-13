#include "disassembler.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "usings.h"
#include <iostream>

namespace emu::synacor {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::string::hexify_wo_0x;

Disassembler::Disassembler(EmulatorMemory<Address, RawData>& memory, std::ostream& ostream)
    : m_memory(memory)
    , m_memory_size(memory.size())
    , m_pc(0)
    , m_ostream(ostream)
{
}

void Disassembler::disassemble()
{
    while (m_pc.underlying() < m_memory_size) {
        print_next_instruction();
    }
    print_next_instruction();
}

void Disassembler::print_next_instruction()
{
    m_ostream
        << hexify_wo_0x(static_cast<u16>(m_pc.underlying()))
        << " (" << hexify_wo_0x(static_cast<u16>(2 * m_pc.underlying()))
        << ")\t\t";

    m_opcode = Data(get_next_value().underlying());

    switch (m_opcode.underlying()) {
    case HALT:
        print_halt(m_ostream);
        break;
    case SET: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_set(m_ostream, farg, sarg);
        break;
    }
    case PUSH: {
        const RawData farg = get_next_value();
        print_push(m_ostream, farg);
        break;
    }
    case POP: {
        const RawData farg = get_next_value();
        print_pop(m_ostream, farg);
        break;
    }
    case EQ: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_eq(m_ostream, farg, sarg, targ);
        break;
    }
    case GT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_gt(m_ostream, farg, sarg, targ);
        break;
    }
    case JMP: {
        const RawData farg = get_next_value();
        print_jmp(m_ostream, farg);
        break;
    }
    case JT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_jt(m_ostream, farg, sarg);
        break;
    }
    case JF: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_jf(m_ostream, farg, sarg);
        break;
    }
    case ADD: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_add(m_ostream, farg, sarg, targ);
        break;
    }
    case MULT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_mult(m_ostream, farg, sarg, targ);
        break;
    }
    case MOD: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_mod(m_ostream, farg, sarg, targ);
        break;
    }
    case AND: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_and(m_ostream, farg, sarg, targ);
        break;
    }
    case OR: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        print_or(m_ostream, farg, sarg, targ);
        break;
    }
    case NOT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_not(m_ostream, farg, sarg);
        break;
    }
    case RMEM: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_rmem(m_ostream, farg, sarg);
        break;
    }
    case WMEM: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        print_wmem(m_ostream, farg, sarg);
        break;
    }
    case CALL: {
        const RawData farg = get_next_value();
        print_call(m_ostream, farg);
        break;
    }
    case RET:
        print_ret(m_ostream);
        break;
    case OUT: {
        const RawData farg = get_next_value();
        print_out(m_ostream, farg);
        break;
    }
    case IN: {
        const RawData farg = get_next_value();
        print_in(m_ostream, farg);
        break;
    }
    case NOOP:
        print_noop(m_ostream);
        break;
    default:
        m_ostream << "db " << hexify_wo_0x(static_cast<u16>(m_opcode.underlying()));
        //throw UnrecognizedOpcodeException(static_cast<u16>(m_opcode.underlying()));
    }

    m_ostream << "\n";
}

RawData Disassembler::get_next_value()
{
    return m_memory.read(m_pc++);
}

}
