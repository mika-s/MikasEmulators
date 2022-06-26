#ifndef MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_SESSION_H
#define MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_SESSION_H

#include <cstdint>
#include <vector>
#include <memory>
#include "8080/cpu.h"
#include "8080/emulator_memory.h"
#include "8080/interfaces/emulator8080.h"
#include "8080/interfaces/out_observer.h"
#include "8080/interfaces/session.h"
#include "crosscutting/typedefs.h"

namespace emu::cpu8080::applications::cpm {

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
        static constexpr int finished_port = 0;
        static constexpr int output_port = 1;
        static constexpr int C_WRITE = 2;
        static constexpr int C_WRITESTR = 9;

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

#endif //MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_SESSION_H
