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
#include <string>

namespace emu::lmc {

    using emu::exceptions::UnrecognizedOpcodeException;
    using emu::util::byte::high_byte;
    using emu::util::byte::low_byte;
    using emu::util::byte::to_u16;
    using emu::util::string::hexify;

    Cpu::Cpu(
            EmulatorMemory &memory,
            const u8 initial_pc
    ) : m_is_halted(false),
        m_memory(memory), m_memory_size(memory.size()),
        m_opcode(0), m_sp(0xffff), m_pc(initial_pc),
        m_acc_reg(0xff) {}

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
        m_acc_reg = 0xff;
        m_pc = 0;
        m_sp = 0xffff;
        m_is_halted = false;
        std::fill(m_io_in.begin(), m_io_in.end(), 0);
        std::fill(m_io_out.begin(), m_io_out.end(), 0);
    }

    void Cpu::start() {
    }

    void Cpu::stop() {
        reset_state();
    }

    Opcode find_opcode([[maybe_unused]] u8 raw_opcode) {
        return Opcode::ADD;
    }

    cyc Cpu::next_instruction() {
        u8 raw_opcode = 0;
        Opcode opcode = find_opcode(raw_opcode);

        switch (opcode) {
            case Opcode::ADD:
                add(m_acc_reg, 0, m_memory);
                break;
            case Opcode::SUB:
                break;
            case Opcode::STA:
                break;
            case Opcode::LDA:
                break;
            case Opcode::BRA:
                break;
            case Opcode::BRZ:
                break;
            case Opcode::BRP:
                break;
            case Opcode::INP:
                break;
            case Opcode::OUT:
                break;
            case Opcode::HLT:
                break;
            default:
                throw UnrecognizedOpcodeException(raw_opcode);
        }

        return 0;
    }

    NextByte Cpu::get_next_byte() {
        return {
                .farg = m_memory.read(m_pc++)};
    }

    NextWord Cpu::get_next_word() {
        return {
                .farg = m_memory.read(m_pc++),
                .sarg = m_memory.read(m_pc++)};
    }

    EmulatorMemory &Cpu::memory() {
        return m_memory;
    }

    u8 Cpu::pc() const {
        return m_pc;
    }

    u8 Cpu::sp() const {
        return m_sp;
    }

    u8 Cpu::a() const {
        return m_acc_reg;
    }

    void Cpu::notify_out_observers(u8 port) {
        for (OutObserver *observer: m_out_observers) {
            observer->out_changed(port);
        }
    }

    void Cpu::notify_in_observers(u8 port) {
        for (InObserver *observer: m_in_observers) {
            observer->in_requested(port);
        }
    }

    void Cpu::print_debug(u8 opcode) {
        if (false) {
            std::cout << "pc=" << hexify(m_pc)
                      << ",sp=" << hexify(m_sp)
                      << ",op=" << hexify(opcode)
                      << ",a=" << hexify(m_acc_reg)
                      << "\n"
                      << std::flush;
        }
    }
}
