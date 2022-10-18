#include <iostream>
#include "cpm_application_session.h"
#include "crosscutting/util/byte_util.h"

namespace emu::applications::cpm::z80 {

    using emu::util::byte::to_u16;

    CpmApplicationSession::CpmApplicationSession(
            std::string loaded_file,
            EmulatorMemory memory
    )
            : m_memory(std::move(memory)),
              m_loaded_file(std::move(loaded_file)),
              m_is_finished(false) {
        setup_cpu();
    }

    void CpmApplicationSession::run() {
        std::cout << "--------------- Starting " << m_loaded_file << " ---------------\n\n";
        m_cpu->start();

        while (m_cpu->can_run_next_instruction() && !m_is_finished) {
            m_cpu->next_instruction();
        }

        m_cpu->stop();
        std::cout << "\n\n--------------- Finished " << m_loaded_file << " ---------------\n\n";
    }

    void CpmApplicationSession::pause() {
        throw std::runtime_error("Pause is not implemented for CP/M programs");
    }

    void CpmApplicationSession::stop() {
        throw std::runtime_error("Stop is not implemented for CP/M programs");
    }

    void CpmApplicationSession::out_changed(u8 port) {
        if (port == finished_port) {
            m_is_finished = true;
        } else if (port == output_port) {
            const u8 operation = m_cpu->c();

            if (operation == C_WRITE) {
                c_write(m_cpu->e());
            } else if (operation == C_WRITESTR) {
                c_writestr(m_cpu->memory(), to_u16(m_cpu->d(), m_cpu->e()));
            }
        } else {
            throw std::runtime_error("Illegal output port");
        }
    }

    void CpmApplicationSession::setup_cpu() {
        const u16 initial_pc = 0x100;

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
    }

    void CpmApplicationSession::c_write(u8 e) {
        std::cout << e;
    }

    void CpmApplicationSession::c_writestr(const EmulatorMemory &memory, u16 address) {
        do {
            std::cout << memory[address++];
        } while (memory[address] != '$');
    }
}
