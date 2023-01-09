#pragma once

#include "applications/cpm_8080/usage.h"
#include "applications/cpm_z80/usage.h"
#include "applications/lmc_application/usage.h"
#include "applications/pacman/usage.h"
#include "applications/space_invaders/usage.h"
#include "applications/zxspectrum_48k/usage.h"
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
    static void run(Options const& options);

    static void print_main_usage(std::string const& program_name);

    static void print_run_usage(std::string const& program_name);

private:
    static constexpr std::size_t s_padding_to_description = 22;

    static const inline std::vector<std::pair<std::string, std::string>> s_supported_programs = {
        { "pacman", "Midway Pacman for Z80" },
        { "zx-spectrum-48k", "ZX Spectrum 48k with Z80" },
        { "prelim", "The prelim CP/M-based test binary for Z80" },
        { "zexall", "The zexall CP/M-based test binary for Z80" },
        { "zexdoc", "The zexdoc CP/M-based test binary for Z80" },
        { "NEWLINE", "NEWLINE" },
        { "space_invaders", "Taito Space Invaders for 8080" },
        { "TST8080", "The TST8080 CP/M-based test binary for 8080" },
        { "8080PRE", "The 8080PRE CP/M-based test binary for 8080" },
        { "8080EXM", "The 8080EXM CP/M-based test binary for 8080" },
        { "CPUTEST", "The CPUTEST CP/M-based test binary for 8080" },
        { "NEWLINE", "NEWLINE" },
        { "lmc_application", "LMC applications" },
    };

    static const inline std::vector<std::pair<std::string, std::string>> s_supported_cpus = {
        { "8080", "The Intel 8080 8-bit microprocessor" },
        { "Z80", "The Zilog Z80 8-bit microprocessor" },
        { "LMC", "The Little Man Computer instructional model" },
    };

    static const inline std::vector<std::pair<std::string, std::string>> s_supported_formats = {
        { "SX_Spectrum_Z80", "The Z80 format for SX Spectrum (Z80 CPU only)" },
    };

    static const inline std::vector<std::pair<std::string, std::string>> s_command_descriptions = {
        { "run", "Run an application" },
        { "disassemble", "Disassemble a binary" },
        { "test", "Run unit tests" },
    };

    static const inline std::vector<std::pair<std::string, std::string>> s_test_examples = {
        { "--cpu=8080", "The unit tests for 8080 are run" },
        { "--cpu=8080 --cpu=Z80", "The unit tests for 8080 and Z80 are run" },
        { "", "All the unit tests are run" },
    };

    static const inline std::unordered_map<std::string, std::function<void(std::string const&)>> s_program_usages = {
        { "pacman", pacman::print_usage },
        { "zx-spectrum-48k", zxspectrum_48k::print_usage },
        { "prelim", cpm::z80::print_usage },
        { "zexall", cpm::z80::print_usage },
        { "zexdoc", cpm::z80::print_usage },
        { "space_invaders", space_invaders::print_usage },
        { "TST8080", cpm::i8080::print_usage },
        { "8080PRE", cpm::i8080::print_usage },
        { "8080EXM", cpm::i8080::print_usage },
        { "CPUTEST", cpm::i8080::print_usage },
        { "lmc_application", lmc_application::print_usage },
    };

    static void run_program(Options const& options);

    static void disassemble(Options const& options);

    static void test(Options const& options);

    static void print_disassemble_usage(std::string const& program_name);

    static void print_test_usage(std::string const& program_name);

    static std::unique_ptr<Emulator> choose_emulator(std::string const& program, Options const& options);

    static bool is_supporting(std::string const& program);
};
}
