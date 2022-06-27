#ifndef MIKA_EMULATORS_Z80_APPLICATIONS_CPM_CPM_APPLICATION_H
#define MIKA_EMULATORS_Z80_APPLICATIONS_CPM_CPM_APPLICATION_H

#include <cstdint>
#include <vector>
#include <memory>
#include "z80/cpu.h"
#include "z80/interfaces/emulatorZ80.h"
#include "z80/interfaces/out_observer.h"
#include "z80/interfaces/session.h"
#include "crosscutting/typedefs.h"

namespace emu::z80::applications::cpm {

    class CpmApplication : public EmulatorZ80 {
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

#endif //MIKA_EMULATORS_Z80_APPLICATIONS_CPM_CPM_APPLICATION_H
