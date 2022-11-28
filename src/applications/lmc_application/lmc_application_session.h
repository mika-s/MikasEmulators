#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H

#include "chips/trivial/lmc/cpu.h"
#include "chips/trivial/lmc/interfaces/in_observer.h"
#include "chips/trivial/lmc/interfaces/out_observer.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/misc/uinteger.h"
#include <memory>
#include <string>

namespace emu::applications::lmc {

    using emu::lmc::Address;
    using emu::lmc::Cpu;
    using emu::lmc::Data;
    using emu::lmc::InObserver;
    using emu::lmc::OutObserver;
    using emu::lmc::OutType;
    using emu::memory::EmulatorMemory;
    using emu::misc::Session;
    using emu::misc::UInteger;

    class LmcApplicationSession : public Session, public OutObserver, public InObserver {
    public:
        LmcApplicationSession(
                std::string loaded_file,
                EmulatorMemory<Address, Data> m_memory
        );

        void run() override;

        void pause() override;

        void stop() override;

        void out_changed(Data acc_reg, OutType out_type) override;

        void in_requested() override;

    private:
        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory<Address, Data> m_memory;
        std::string m_loaded_file;

        void setup_cpu();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
