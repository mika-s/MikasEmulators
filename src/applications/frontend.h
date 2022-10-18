#ifndef MIKA_EMULATORS_APPLICATIONS_FRONTEND_H
#define MIKA_EMULATORS_APPLICATIONS_FRONTEND_H

#include <vector>
#include <memory>
#include "src/chips/z80/cpu.h"
#include "src/chips/8080/cpu.h"
#include "applications/options.h"
#include "applications/pacman/pacman.h"
#include "applications/pacman/settings.h"
#include "applications/space_invaders/space_invaders.h"
#include "applications/space_invaders/settings.h"
#include "crosscutting/gui/gui_type.h"
#include "crosscutting/misc/emulator.h"

namespace emu::applications {

    using emu::gui::GuiType;
    using emu::misc::Emulator;

    class Frontend {
    public:
        static void run(Options &options);

        static std::string supported();

    private:
        static constexpr unsigned int global__minimum_number_of_arguments = 2;
        static constexpr unsigned int mode_argument = 1;

        static constexpr unsigned int run__minimum_number_of_arguments = 3;
        static constexpr unsigned int run__program_argument = 2;

        static constexpr unsigned int disassembly__number_of_arguments = 4;
        static constexpr unsigned int disassembly__cpu_argument = 2;
        static constexpr unsigned int disassembly__file_path_argument = 3;

        static constexpr unsigned int test__minimum_number_of_arguments = 2;
        static constexpr unsigned int test__number_of_arguments_when_no_cpus_provided = 2;

        static const inline std::vector<std::string> supported_programs = {
                "pacman",
                "prelim",
                "zexall",
                "zexdoc",

                "space_invaders",
                "TST8080",
                "8080PRE",
                "8080EXM",
                "CPUTEST"
        };

        static void run(const std::vector<std::string> &args, Options &options);

        static void disassemble(const std::vector<std::string> &args);

        static void test(const std::vector<std::string> &args);

        static std::unique_ptr<Emulator> choose_emulator(const std::string &program, Options &options);

        static bool is_supporting(const std::string &program);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_FRONTEND_H
