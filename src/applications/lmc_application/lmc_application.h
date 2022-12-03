#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H

#include "chips/trivial/lmc/cpu.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/uinteger.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::lmc {
    class Ui;
}
namespace emu::applications::lmc {
    class Input;
}
namespace emu::misc {
    class Session;
}

namespace emu::applications::lmc {

    using emu::gui::GuiType;
    using emu::lmc::Address;
    using emu::lmc::Cpu;
    using emu::lmc::Data;
    using emu::memory::EmulatorMemory;
    using emu::misc::Emulator;
    using emu::misc::RunStatus;
    using emu::misc::Session;
    using emu::misc::UInteger;

    class LmcApplication : public Emulator {
    public:
        explicit LmcApplication(const std::string &file, const GuiType gui_type);

        std::unique_ptr<Session> new_session() override;

    private:
        static constexpr std::size_t memory_size = 100;

        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory<Address, Data> m_memory;
        std::shared_ptr<Ui> m_gui;
        std::shared_ptr<Input> m_input;
        RunStatus m_startup_runstatus;
        std::string m_loaded_file;

        void load_file(const std::string &file);

        std::vector<Data> create_work_ram(std::size_t size);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_H
