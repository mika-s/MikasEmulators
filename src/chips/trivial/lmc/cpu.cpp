#include "cpu.h"
#include "crosscutting/exceptions/unrecognized_opcode_exception.h"
#include "crosscutting/memory/emulator_memory.h"
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
            EmulatorMemory<u8, u16> &memory,
            const u8 initial_pc
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
        return m_pc < m_memory_size;
    }

    void Cpu::reset_state() {
        m_acc_reg = 0;
        m_pc = 0;
        m_is_halted = false;
        std::fill(m_io_in.begin(), m_io_in.end(), 0);
        std::fill(m_io_out.begin(), m_io_out.end(), 0);
    }

    void Cpu::start() {
    }

    void Cpu::stop() {
        reset_state();
    }

    Opcode find_opcode(u16 raw_opcode) {
        if (raw_opcode == 0) {
            return Opcode::HLT;
        } else if (100 <= raw_opcode && raw_opcode <= 199) {
            return Opcode::ADD;
        } else if (200 <= raw_opcode && raw_opcode <= 299) {
            return Opcode::SUB;
        } else if (300 <= raw_opcode && raw_opcode <= 399) {
            return Opcode::STA;
        } else if (500 <= raw_opcode && raw_opcode <= 599) {
            return Opcode::LDA;
        } else if (600 <= raw_opcode && raw_opcode <= 699) {
            return Opcode::BRA;
        } else if (700 <= raw_opcode && raw_opcode <= 799) {
            return Opcode::BRZ;
        } else if (800 <= raw_opcode && raw_opcode <= 899) {
            return Opcode::BRP;
        } else if (raw_opcode == 901) {
            return Opcode::INP;
        } else if (raw_opcode <= 902) {
            return Opcode::OUT;
        } else {
            throw UnrecognizedOpcodeException(raw_opcode);
        }
    }

    u8 find_argument(u16 raw_opcode) {
        return raw_opcode % 100;
    }

    cyc Cpu::next_instruction() {
        u16 raw_opcode = get_next_value();
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

        return 0;
    }

    u16 Cpu::get_next_value() {
        return m_memory.read(m_pc++);
    }

    EmulatorMemory<u8, u16> &Cpu::memory() {
        return m_memory;
    }

    u16 Cpu::a() const {
        return m_acc_reg;
    }

    u8 Cpu::pc() const {
        return m_pc;
    }

    void Cpu::input(u16 value) {
        m_acc_reg = value;
    }

    void Cpu::notify_out_observers(u16 acc_reg) {
        for (OutObserver *observer: m_out_observers) {
            observer->out_changed(acc_reg);
        }
    }

    void Cpu::notify_in_observers() {
        for (InObserver *observer: m_in_observers) {
            observer->in_requested();
        }
    }

    void Cpu::print_debug(u16 opcode) {
        if (false) {
            std::cout << "pc=" << +m_pc
                      << ",op=" << +opcode
                      << ",a=" << +m_acc_reg
                      << ",nf=" << m_flag_reg.is_negative_flag_set()
                      << "\n"
                      << std::flush;
        }
    }
}
