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

Disassembler::Disassembler(EmulatorMemory<Address, Data>& memory, std::ostream& ostream)
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
    m_ostream << hexify_wo_0x(static_cast<u16>(m_pc.underlying())) << "\t\t";

    m_opcode = get_next_value();

    switch (m_opcode.underlying()) {
    case HALT:
        print_halt(m_ostream);
        break;
    case SET:
        print_set(m_ostream, get_next_value(), get_next_value());
        break;
    case PUSH:
        print_push(m_ostream, Address(get_next_value()));
        break;
    case POP:
        print_pop(m_ostream, Address(get_next_value()));
        break;
    case EQ:
        print_eq(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case GT:
        print_gt(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case JMP:
        print_jmp(m_ostream, Address(get_next_value()));
        break;
    case JT:
        print_jt(m_ostream, get_next_value(), Address(get_next_value()));
        break;
    case JF:
        print_jf(m_ostream, get_next_value(), Address(get_next_value()));
        break;
    case ADD:
        print_add(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case MULT:
        print_mult(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case MOD:
        print_mod(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case AND:
        print_and(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case OR:
        print_or(m_ostream, Address(get_next_value()), get_next_value(), get_next_value());
        break;
    case NOT:
        print_not(m_ostream, Address(get_next_value()), get_next_value());
        break;
    case RMEM:
        print_rmem(m_ostream, Address(get_next_value()), Address(get_next_value()));
        break;
    case WMEM:
        print_wmem(m_ostream, Address(get_next_value()), Address(get_next_value()));
        break;
    case CALL:
        print_call(m_ostream, Address(get_next_value()));
        break;
    case RET:
        print_ret(m_ostream);
        break;
    case OUT:
        print_out(m_ostream, get_next_value());
        break;
    case IN:
        print_in(m_ostream, Address(get_next_value()));
        break;
    case NOOP:
        print_noop(m_ostream);
        break;
    default:
        throw UnrecognizedOpcodeException(m_opcode.underlying());
    }

    m_ostream << "\n";
}

Data Disassembler::get_next_value()
{
    return m_memory.read(m_pc++);
}

}
