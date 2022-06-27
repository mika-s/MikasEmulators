#ifndef MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H
#define MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H

#include <cstdint>
#include <vector>
#include <memory>
#include "8080/cpu.h"
#include "8080/interfaces/emulator8080.h"
#include "8080/interfaces/out_observer.h"
#include "8080/interfaces/session.h"
#include "crosscutting/typedefs.h"

namespace emu::i8080::applications::cpm {

    class CpmApplication : public Emulator8080 {
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

#endif //MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H
