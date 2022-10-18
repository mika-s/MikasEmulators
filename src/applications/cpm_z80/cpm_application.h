#ifndef MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_H
#define MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_H

#include <vector>
#include <memory>
#include "chips/z80/cpu.h"
#include "chips/z80/interfaces/out_observer.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"

namespace emu::applications::cpm::z80 {

    using emu::misc::Emulator;
    using emu::misc::Session;
    using emu::z80::Cpu;
    using emu::z80::EmulatorMemory;

    class CpmApplication : public Emulator {
    public:
        explicit CpmApplication(const std::string &file);

        std::unique_ptr<Session> new_session() override;

    private:
        std::unique_ptr<Cpu> m_cpu;
        EmulatorMemory m_memory;
        std::string m_loaded_file;

        void load_file(const std::string &file);

        static std::vector<u8> create_initial_offset();

        static std::vector<u8> create_work_ram(size_t size);

        static void patch_program(EmulatorMemory &program);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_CPM_Z80_CPM_APPLICATION_H
