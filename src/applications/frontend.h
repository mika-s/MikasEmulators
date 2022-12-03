#ifndef MIKA_EMULATORS_APPLICATIONS_FRONTEND_H
#define MIKA_EMULATORS_APPLICATIONS_FRONTEND_H

#include "applications/cpm_8080/usage.h"
#include "applications/cpm_z80/usage.h"
#include "applications/lmc_application/usage.h"
#include "applications/pacman/usage.h"
#include "applications/space_invaders/usage.h"
#include "crosscutting/gui/gui_type.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications {
    class Options;
}
namespace emu::misc {
    class Emulator;
}

namespace emu::applications {

    using emu::gui::GuiType;
    using emu::misc::Emulator;

    class Frontend {
    public:
        static void run(const Options &options);

        static void print_main_usage(const std::string &program_name);

        static void print_run_usage(const std::string &program_name);

    private:
        static constexpr std::size_t padding_to_description = 22;

        static const inline std::vector<std::pair<std::string, std::string>> supported_programs = {
                {"pacman", "Midway Pacman for Z80"},
                {"prelim", "The prelim CP/M-based test binary for Z80"},
                {"zexall", "The zexall CP/M-based test binary for Z80"},
                {"zexdoc", "The zexdoc CP/M-based test binary for Z80"},
                {"NEWLINE", "NEWLINE"},
                {"space_invaders", "Taito Space Invaders for 8080"},
                {"TST8080", "The TST8080 CP/M-based test binary for 8080"},
                {"8080PRE", "The 8080PRE CP/M-based test binary for 8080"},
                {"8080EXM", "The 8080EXM CP/M-based test binary for 8080"},
                {"CPUTEST", "The CPUTEST CP/M-based test binary for 8080"},
                {"NEWLINE", "NEWLINE"},
                {"lmc_application", "LMC applications"},
        };

        static const inline std::vector<std::pair<std::string, std::string>> supported_cpus = {
                {"8080", "The Intel 8080 8-bit microprocessor"},
                {"Z80", "The Zilog Z80 8-bit microprocessor"},
                {"LMC", "The Little Man Computer instructional model"},
        };

        static const inline std::vector<std::pair<std::string, std::string>> command_descriptions = {
                {"run", "Run an application"},
                {"disassemble", "Disassemble a binary"},
                {"test", "Run unit tests"},
        };

        static const inline std::vector<std::pair<std::string, std::string>> test_examples = {
                {"--cpu=8080", "The unit tests for 8080 are run"},
                {"--cpu=8080 --cpu=Z80", "The unit tests for 8080 and Z80 are run"},
                {"", "All the unit tests are run"},
        };

        static const inline std::unordered_map<std::string, std::function<void(const std::string &)>> program_usages = {
                {"pacman", pacman::print_usage},
                {"prelim", cpm::z80::print_usage},
                {"zexall", cpm::z80::print_usage},
                {"zexdoc", cpm::z80::print_usage},
                {"space_invaders", space_invaders::print_usage},
                {"TST8080", cpm::i8080::print_usage},
                {"8080PRE", cpm::i8080::print_usage},
                {"8080EXM", cpm::i8080::print_usage},
                {"CPUTEST", cpm::i8080::print_usage},
                {"lmc_application", lmc_application::print_usage},
        };

        static void run_program(const Options &options);

        static void disassemble(const Options &options);

        static void test(const Options &options);

        static void print_disassemble_usage(const std::string &program_name);

        static void print_test_usage(const std::string &program_name);

        static std::unique_ptr<Emulator> choose_emulator(const std::string &program, const Options &options);

        static bool is_supporting(const std::string &program);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_FRONTEND_H
