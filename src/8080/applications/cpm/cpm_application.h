#ifndef MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H
#define MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H

#include <cstdint>
#include <vector>
#include "8080/cpu.h"
#include "8080/interfaces/emulator8080.h"
#include "8080/interfaces/out_observer.h"

namespace emu::cpu8080::applications::cpm {
    class CpmApplication : public Emulator8080, public OutObserver {
    public:
        explicit CpmApplication(const std::string &file);

        ~CpmApplication() override;

        void run() override;

        void out_changed(std::uint8_t port) override;

    private:
        static constexpr int C_WRITE = 2;
        static constexpr int C_WRITESTR = 9;

        Cpu *cpu;

        std::string loaded_file;

        emu::cpu8080::EmulatorMemory memory;

        bool finished;

        void load_file(const std::string &file);

        static std::vector<std::uint8_t> create_initial_offset();

        static std::vector<std::uint8_t> create_work_ram(size_t size);

        static void patch_program(emu::cpu8080::EmulatorMemory &program);

        // CP/M syscalls
        // https://www.seasip.info/Cpm/bdos.html

        static void c_write(std::uint8_t e);

        static void c_writestr(const emu::cpu8080::EmulatorMemory &memory, std::uint16_t address);
    };
}

#endif //MIKA_EMULATORS_8080_APPLICATIONS_CPM_CPM_APPLICATION_H
