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
    EmulatorMemory<Address, RawData>& memory,
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
    return m_pc < Address(m_memory.size()) && !m_is_halted;
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
    m_opcode = Data(get_next_value().underlying());

    print_debug(m_opcode);

    switch (m_opcode.underlying()) {
    case HALT:
        halt(m_is_halted);
        break;
    case SET: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        set(m_memory, farg, sarg);
        break;
    }
    case PUSH: {
        const RawData farg = get_next_value();
        push(m_stack, m_memory, farg);
        break;
    }
    case POP: {
        const RawData farg = get_next_value();
        pop(m_stack, m_memory, farg);
        break;
    }
    case EQ: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        eq(m_memory, farg, sarg, targ);
        break;
    }
    case GT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        gt(m_memory, farg, sarg, targ);
        break;
    }
    case JMP: {
        const RawData farg = get_next_value();
        jmp(m_pc, farg);
        break;
    }
    case JT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        jt(m_pc, m_memory, farg, sarg);
        break;
    }
    case JF: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        jf(m_pc, m_memory, farg, sarg);
        break;
    }
    case ADD: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        add(m_memory, farg, sarg, targ);
        break;
    }
    case MULT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        mult(m_memory, farg, sarg, targ);
        break;
    }
    case MOD: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        mod(m_memory, farg, sarg, targ);
        break;
    }
    case AND: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        and_(m_memory, farg, sarg, targ);
        break;
    }
    case OR: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        const RawData targ = get_next_value();
        or_(m_memory, farg, sarg, targ);
        break;
    }
    case NOT: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        not_(m_memory, farg, sarg);
        break;
    }
    case RMEM: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        rmem(m_memory, farg, sarg);
        break;
    }
    case WMEM: {
        const RawData farg = get_next_value();
        const RawData sarg = get_next_value();
        wmem(m_memory, farg, sarg);
        break;
    }
    case CALL: {
        const RawData farg = get_next_value();
        call(m_pc, m_stack, farg);
        break;
    }
    case RET: {
        ret(m_stack, m_pc, m_is_halted);
        break;
    }
    case OUT: {
        const RawData farg = get_next_value();
        out(Data(farg.underlying()));
        notify_out_observers(Data(farg.underlying()));
        break;
    }
    case IN: {
        in(Address(get_next_value()), m_memory);
        break;
    }
    case NOOP:
        noop();
        break;
    default:
        throw UnrecognizedOpcodeException(static_cast<u16>(m_opcode.underlying()));
    }
}

RawData Cpu::get_next_value()
{
    return m_memory.read(m_pc++);
}

EmulatorMemory<Address, RawData>& Cpu::memory()
{
    return m_memory;
}

Address Cpu::pc() const
{
    return m_pc;
}

void Cpu::input([[maybe_unused]] Data value)
{
    //    m_r0 = value;
}

void Cpu::notify_out_observers(Data character)
{
    for (OutObserver* observer : m_out_observers) {
        observer->out_changed(character);
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
                  //                  << ",r0=" << m_r0.underlying()
                  //                  << ",r1=" << m_r1.underlying()
                  //                  << ",r2=" << m_r2.underlying()
                  //                  << ",r3=" << m_r3.underlying()
                  //                  << ",r4=" << m_r4.underlying()
                  //                  << ",r5=" << m_r5.underlying()
                  //                  << ",r6=" << m_r6.underlying()
                  //                  << ",r7=" << m_r7.underlying()
                  << "\n"
                  << std::flush;
    }
}
}
