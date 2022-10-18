#ifndef MIKA_EMULATORS_APPLICATIONS_CPM_8080_CPM_APPLICATION_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_CPM_8080_CPM_APPLICATION_SESSION_H

#include <vector>
#include <memory>
#include "chips/8080/cpu.h"
#include "chips/8080/emulator_memory.h"
#include "chips/8080/interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"

namespace emu::applications::cpm::i8080 {

    using emu::i8080::Cpu;
    using emu::i8080::EmulatorMemory;
    using emu::i8080::OutObserver;
    using emu::misc::Emulator;
    using emu::misc::Session;

    class CpmApplicationSession : public Session, public OutObserver {
    public:
        CpmApplicationSession(
                std::string loaded_file,
                EmulatorMemory m_memory
        );

        void run() override;

        void pause() override;

        void stop() override;

        void out_changed(u8 port) override;

    private:
        static constexpr u8 finished_port = 0;
        static constexpr u8 output_port = 1;
        static constexpr u8 C_WRITE = 2;
        static constexpr u8 C_WRITESTR = 9;

        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory m_memory;
        std::string m_loaded_file;
        bool m_is_finished;

        void setup_cpu();

        // CP/M syscalls
        // https://www.seasip.info/Cpm/bdos.html

        static void c_write(u8 e);

        static void c_writestr(const EmulatorMemory &memory, u16 address);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_CPM_8080_CPM_APPLICATION_SESSION_H
