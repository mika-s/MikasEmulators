#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H

#include "chips/z80/cpu.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>

namespace emu::misc {
    class Session;
}

namespace emu::applications::lmc {

    using emu::misc::Emulator;
    using emu::misc::Session;
    using emu::z80::Cpu;
    using emu::z80::EmulatorMemory;

    class LmcApplication : public Emulator {
    public:
        explicit LmcApplication(const std::string &file);

        std::unique_ptr<Session> new_session() override;

    private:
        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory<u16, u8> m_memory;
        std::string m_loaded_file;

        void load_file(const std::string &file);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H
