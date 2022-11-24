#include "cpu.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "crosscutting/util/string_util.h"
#include "instructions/instructions.h"
#include "interfaces/in_observer.h"
#include "interfaces/out_observer.h"
#include "opcode.h"
#include <algorithm>
#include <iostream>

namespace emu::lmc {

    using emu::exceptions::UnrecognizedOpcodeException;
    using emu::util::byte::high_byte;
    using emu::util::byte::low_byte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify;

    Cpu::Cpu(
            EmulatorMemory<Address, Data> &memory,
            const Address initial_pc
    ) : m_is_halted(false),
        m_memory(memory), m_memory_size(memory.size()),
        m_pc(initial_pc),
        m_acc_reg(0) {}

    Cpu::~Cpu() {
        m_out_observers.clear();
        m_in_observers.clear();
    }

    void Cpu::add_out_observer(OutObserver &observer) {
        m_out_observers.push_back(&observer);
    }

    void Cpu::remove_out_observer(OutObserver *observer) {
        m_out_observers.erase(
                std::remove(m_out_observers.begin(), m_out_observers.end(), observer),
                m_out_observers.end()
        );
    }

    void Cpu::add_in_observer(InObserver &observer) {
        m_in_observers.push_back(&observer);
    }

    void Cpu::remove_in_observer(InObserver *observer) {
        m_in_observers.erase(
                std::remove(m_in_observers.begin(), m_in_observers.end(), observer),
                m_in_observers.end()
        );
    }

    bool Cpu::can_run_next_instruction() const {
        return m_pc.underlying() < m_memory_size;
    }

    void Cpu::reset_state() {
        m_acc_reg = Data(0);
        m_pc = Address(0);
        m_is_halted = false;
    }

    void Cpu::start() {
    }

    void Cpu::stop() {
        reset_state();
    }

    Opcode find_opcode(Data raw_opcode) {
        u64 opcode = raw_opcode.underlying();
        if (opcode == 0) {
            return Opcode::HLT;
        } else if (100 <= opcode && opcode <= 199) {
            return Opcode::ADD;
        } else if (200 <= opcode && opcode <= 299) {
            return Opcode::SUB;
        } else if (300 <= opcode && opcode <= 399) {
            return Opcode::STA;
        } else if (500 <= opcode && opcode <= 599) {
            return Opcode::LDA;
        } else if (600 <= opcode && opcode <= 699) {
            return Opcode::BRA;
        } else if (700 <= opcode && opcode <= 799) {
            return Opcode::BRZ;
        } else if (800 <= opcode && opcode <= 899) {
            return Opcode::BRP;
        } else if (opcode == 901) {
            return Opcode::INP;
        } else if (opcode <= 902) {
            return Opcode::OUT;
        } else {
            throw UnrecognizedOpcodeException(opcode);
        }
    }

    Address find_argument(Data raw_opcode) {
        return Address(raw_opcode.underlying()); // TODO
    }

    void Cpu::next_instruction() {
        Data raw_opcode = get_next_value();
        const Opcode opcode = find_opcode(raw_opcode);

        print_debug(raw_opcode);

        switch (opcode) {
            case Opcode::ADD:
                add(m_acc_reg, find_argument(raw_opcode), m_memory);
                break;
            case Opcode::SUB:
                sub(m_acc_reg, find_argument(raw_opcode), m_memory, m_flag_reg);
                break;
            case Opcode::STA:
                sta(m_acc_reg, find_argument(raw_opcode), m_memory);
                break;
            case Opcode::LDA:
                lda(m_acc_reg, find_argument(raw_opcode), m_memory);
                break;
            case Opcode::BRA:
                bra(m_pc, find_argument(raw_opcode));
                break;
            case Opcode::BRZ:
                brz(m_acc_reg, m_pc, find_argument(raw_opcode));
                break;
            case Opcode::BRP:
                brp(m_pc, find_argument(raw_opcode), m_flag_reg);
                break;
            case Opcode::INP:
                notify_in_observers();
                break;
            case Opcode::OUT:
                notify_out_observers(m_acc_reg);
                break;
            case Opcode::HLT:
                hlt(m_is_halted);
                break;
        }
    }

    Data Cpu::get_next_value() {
        return m_memory.read(m_pc++);
    }

    EmulatorMemory<Address, Data> &Cpu::memory() {
        return m_memory;
    }

    Data Cpu::a() const {
        return m_acc_reg;
    }

    Address Cpu::pc() const {
        return m_pc;
    }

    void Cpu::input(Data value) {
        m_acc_reg = value;
    }

    void Cpu::notify_out_observers(Data acc_reg) {
        for (OutObserver *observer: m_out_observers) {
            observer->out_changed(acc_reg);
        }
    }

    void Cpu::notify_in_observers() {
        for (InObserver *observer: m_in_observers) {
            observer->in_requested();
        }
    }

    void Cpu::print_debug([[maybe_unused]] Data opcode) {
        if (false) {
            std::cout << "pc=" << m_pc.underlying()
                      << ",op=" << opcode.underlying()
                      << ",a=" << m_acc_reg.underlying()
                      << ",nf=" << m_flag_reg.is_negative_flag_set()
                      << "\n"
                      << std::flush;
        }
    }
}
