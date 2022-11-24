#include "lmc_application_session.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/byte_util.h"
#include "lmc/cpu.h"
#include <iostream>
#include <stdexcept>
#include <utility>

namespace emu::applications::lmc {

    using emu::lmc::Address;
    using emu::lmc::Data;
    using emu::util::byte::to_u16;

    LmcApplicationSession::LmcApplicationSession(
            std::string loaded_file,
            EmulatorMemory<Address, Data> memory
    )
        : m_memory(std::move(memory)),
          m_loaded_file(std::move(loaded_file)),
          m_is_finished(false) {
        setup_cpu();
    }

    void LmcApplicationSession::run() {
        std::cout << "--------------- Starting " << m_loaded_file << " ---------------\n\n";
        m_cpu->start();

        while (m_cpu->can_run_next_instruction() && !m_is_finished) {
            m_cpu->next_instruction();
        }

        m_cpu->stop();
        std::cout << "\n\n--------------- Finished " << m_loaded_file << " ---------------\n\n";
    }

    void LmcApplicationSession::pause() {
        throw std::runtime_error("Pause is not implemented for LMC programs");
    }

    void LmcApplicationSession::stop() {
        throw std::runtime_error("Stop is not implemented for LMC programs");
    }

    void LmcApplicationSession::out_changed(Data acc_reg) {
        std::cout << acc_reg.underlying();
    }

    void LmcApplicationSession::in_requested() {
        std::cout << "inp: " << std::flush;

        u16 number;
        std::cin >> number;

        m_cpu->input(Data(number));
    }

    void LmcApplicationSession::setup_cpu() {
        const Address initial_pc(0);

        m_cpu = std::make_unique<Cpu>(m_memory, initial_pc);

        m_cpu->add_out_observer(*this);
        m_cpu->add_in_observer(*this);
    }
}
