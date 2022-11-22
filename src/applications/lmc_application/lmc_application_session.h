#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H

#include "chips/trivial/lmc/cpu.h"
#include "chips/trivial/lmc/interfaces/in_observer.h"
#include "chips/trivial/lmc/interfaces/out_observer.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::applications::lmc {

    using emu::lmc::Cpu;
    using emu::lmc::InObserver;
    using emu::lmc::OutObserver;
    using emu::memory::EmulatorMemory;
    using emu::misc::Session;

    class LmcApplicationSession : public Session, public OutObserver, public InObserver {
    public:
        LmcApplicationSession(
                std::string loaded_file,
                EmulatorMemory<u8, u16> m_memory
        );

        void run() override;

        void pause() override;

        void stop() override;

        void out_changed(u16 acc_reg) override;

        void in_requested() override;

    private:
        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory<u8, u16> m_memory;
        std::string m_loaded_file;
        bool m_is_finished;

        void setup_cpu();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
