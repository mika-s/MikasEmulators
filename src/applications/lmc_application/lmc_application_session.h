#ifndef MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H

#include "chips/trivial/lmc/interfaces/gui_observer.h"
#include "chips/trivial/lmc/interfaces/in_observer.h"
#include "chips/trivial/lmc/interfaces/out_observer.h"
#include "chips/trivial/lmc/out_type.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/debugging/debug_container.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/governor.h"
#include "crosscutting/misc/run_status.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include <memory>
#include <string>
#include <vector>

namespace emu::applications::lmc {
    class Ui;
}
namespace emu::applications::lmc {
    class Input;
}
namespace emu::debugger {
    class Debugger;
}
namespace emu::lmc {
    class Cpu;
}
namespace emu::logging {
    class Logger;
}

namespace emu::applications::lmc {

    using emu::debugger::DebugContainer;
    using emu::debugger::Debugger;
    using emu::lmc::Address;
    using emu::lmc::Cpu;
    using emu::lmc::Data;
    using emu::lmc::GuiObserver;
    using emu::lmc::InObserver;
    using emu::lmc::OutObserver;
    using emu::lmc::OutType;
    using emu::logging::Logger;
    using emu::memory::EmulatorMemory;
    using emu::misc::Governor;
    using emu::misc::RunStatus;
    using emu::misc::Session;
    using emu::misc::UInteger;

    class LmcApplicationSession
        : public Session,
          public GuiObserver,
          public OutObserver,
          public InObserver {
    public:
        LmcApplicationSession(
                const RunStatus startup_runstatus,
                std::shared_ptr<Ui> gui,
                std::shared_ptr<Input> input,
                std::string loaded_file,
                std::string file_content,
                EmulatorMemory<Address, Data> memory
        );

        ~LmcApplicationSession() override;

        void run() override;

        void pause() override;

        void stop() override;

        void run_status_changed(RunStatus new_status) override;

        void debug_mode_changed(bool is_in_debug_mode) override;

        void source_code_changed(const std::string &source) override;

        void assemble_and_load_request() override;

        void out_changed(Data acc_reg, OutType out_type) override;

        void in_requested() override;

    private:
        static constexpr unsigned int program_size = 100;

        // Game loop - begin
        static constexpr long double fps = 60.0L;
        static constexpr long double tick_limit = 1000.0L / fps;
        static constexpr int cycles_per_ms = 2000;
        static constexpr long double cycles_per_tick = cycles_per_ms * tick_limit;
        // Game loop - end

        bool m_is_in_debug_mode;
        bool m_is_stepping_instruction;
        bool m_is_stepping_cycle;
        bool m_is_continuing_execution;
        [[maybe_unused]] RunStatus m_startup_runstatus;
        RunStatus m_run_status;

        std::shared_ptr<Ui> m_gui;
        std::shared_ptr<Input> m_input;
        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory<Address, Data> m_memory;
        std::string m_loaded_file;
        std::string m_file_content;
        std::shared_ptr<Logger> m_logger;
        std::shared_ptr<Debugger> m_debugger;
        DebugContainer<Address, Data> m_debug_container;

        Governor m_governor;

        void running(cyc &cycles);

        void pausing();

        void stepping(cyc &cycles);

        void await_input_and_update_debug();

        void setup_cpu();

        void setup_debugging();

        std::vector<Data> memory();
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_LMC_APPLICATION_SESSION_H
