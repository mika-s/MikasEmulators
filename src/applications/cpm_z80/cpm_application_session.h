#ifndef MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_SESSION_H
#define MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_SESSION_H

#include <vector>
#include <memory>
#include "chips/z80/cpu.h"
#include "chips/z80/emulator_memory.h"
#include "chips/z80/interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/session.h"

namespace emu::applications::cpm::z80 {

    using emu::z80::Cpu;
    using emu::z80::EmulatorMemory;
    using emu::z80::OutObserver;
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

#endif //MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_SESSION_H