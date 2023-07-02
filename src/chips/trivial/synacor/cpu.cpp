#include "cpu.h"
#include "chips/trivial/synacor/usings.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include <algorithm>
#include <iostream>

namespace emu::synacor {

using emu::exceptions::UnrecognizedOpcodeException;
using emu::util::byte::high_byte;
using emu::util::byte::low_byte;
using emu::util::byte::to_u16;
using emu::util::string::hexify;

Cpu::Cpu(
    EmulatorMemory<Address, Data>& memory,
    const Address initial_pc)
    : m_is_halted(false)
    , m_memory(memory)
    , m_pc(initial_pc)
{
}

Cpu::~Cpu()
{
    m_out_observers.clear();
    m_in_observers.clear();
}

void Cpu::add_out_observer(OutObserver& observer)
{
    m_out_observers.push_back(&observer);
}

void Cpu::remove_out_observer(OutObserver* observer)
{
    m_out_observers.erase(
        std::remove(m_out_observers.begin(), m_out_observers.end(), observer),
        m_out_observers.end());
}

void Cpu::add_in_observer(InObserver& observer)
{
    m_in_observers.push_back(&observer);
}

void Cpu::remove_in_observer(InObserver* observer)
{
    m_in_observers.erase(
        std::remove(m_in_observers.begin(), m_in_observers.end(), observer),
        m_in_observers.end());
}

bool Cpu::can_run_next_instruction() const
{
    return m_pc < Address(99) && !m_is_halted;
}

void Cpu::reset_state()
{
    m_pc = Address(0);
    m_is_halted = false;
}

void Cpu::start()
{
}

void Cpu::stop()
{
    reset_state();
}

void Cpu::next_instruction()
{
    m_opcode = get_next_value();

    print_debug(m_opcode);

    switch (m_opcode.underlying()) {
    case HALT:
        halt(m_is_halted);
        break;
    case SET:
        set(m_r0, get_next_value()); // TODO
        break;
    case PUSH:
        push();
        break;
    case POP:
        pop();
        break;
    case EQ:
        eq();
        break;
    case GT:
        gt();
        break;
    case JMP:
        jmp();
        break;
    case JT:
        jt();
        break;
    case JF:
        jf();
        break;
    case ADD:
        add();
        break;
    case MULT:
        mult();
        break;
    case MOD:
        mod();
        break;
    case AND:
        and_();
        break;
    case OR:
        or_();
        break;
    case NOT:
        not_();
        break;
    case RMEM:
        rmem();
        break;
    case WMEM:
        wmem();
        break;
    case CALL:
        call();
        break;
    case RET:
        ret();
        break;
    case OUT:
        out();
        break;
    case IN:
        in();
        break;
    case NOOP:
        noop();
        break;
    default:
        throw UnrecognizedOpcodeException(m_opcode.underlying());
    }
}

Data Cpu::get_next_value()
{
    return m_memory.read(m_pc++);
}

EmulatorMemory<Address, Data>& Cpu::memory()
{
    return m_memory;
}

Data Cpu::r0() const
{
    return m_r0;
}

Data Cpu::r1() const
{
    return m_r1;
}

Data Cpu::r2() const
{
    return m_r2;
}

Data Cpu::r3() const
{
    return m_r3;
}

Data Cpu::r4() const
{
    return m_r4;
}

Data Cpu::r5() const
{
    return m_r5;
}

Data Cpu::r6() const
{
    return m_r6;
}

Data Cpu::r7() const
{
    return m_r7;
}

Address Cpu::pc() const
{
    return m_pc;
}

void Cpu::input(Data value)
{
    m_r0 = value;
}

void Cpu::notify_out_observers(Data acc_reg)
{
    for (OutObserver* observer : m_out_observers) {
        observer->out_changed(acc_reg);
    }
}

void Cpu::notify_in_observers()
{
    for (InObserver* observer : m_in_observers) {
        observer->in_requested();
    }
}

void Cpu::print_debug([[maybe_unused]] Data opcode)
{
    if (false) {
        std::cout << "pc=" << m_pc
                  << ",op=" << opcode
                  << ",r0=" << m_r0.underlying()
                  << ",r1=" << m_r1.underlying()
                  << ",r2=" << m_r2.underlying()
                  << ",r3=" << m_r3.underlying()
                  << ",r4=" << m_r4.underlying()
                  << ",r5=" << m_r5.underlying()
                  << ",r6=" << m_r6.underlying()
                  << ",r7=" << m_r7.underlying()
                  << "\n"
                  << std::flush;
    }
}
}
