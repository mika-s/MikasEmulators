#include "frontend.h"
#include "applications/cpm_8080/cpm_application.h"
#include "applications/cpm_8080/usage.h"
#include "applications/cpm_z80/cpm_application.h"
#include "applications/lmc_application/lmc_application.h"
#include "applications/lmc_application/usage.h"
#include "applications/pacman/pacman.h"
#include "applications/pacman/settings.h"
#include "applications/pacman/usage.h"
#include "applications/space_invaders/settings.h"
#include "applications/space_invaders/space_invaders.h"
#include "applications/space_invaders/usage.h"
#include "chips/8080/disassembler.h"
#include "chips/z80/disassembler.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "options.h"
#include <algorithm>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <iterator>
#include <optional>

namespace emu::applications {

using emu::exceptions::InvalidProgramArgumentsException;
using emu::util::string::create_padding;

void Frontend::run(Options const& options)
{
    std::string const& command = options.command();

    if (command == "run") {
        run_program(options);
    } else if (command == "disassemble") {
        disassemble(options);
    } else if (command == "test") {
        test(options);
    } else {
        throw InvalidProgramArgumentsException(
            fmt::format("Unknown command: {}", command),
            Frontend::print_main_usage);
    }
}

void Frontend::print_main_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " COMMAND [APPLICATION] [FLAGS] [PATH]\n\n";
    std::cout << "A collection of emulators\n\n";

    std::cout << "Commands:\n";

    for (auto& command_description : s_command_descriptions) {
        std::string padding = create_padding(command_description.first.size(), s_padding_to_description);
        std::cout << "  " << command_description.first << padding << command_description.second << "\n";
    }

    std::cout << "\n\nRun './" << program_name << " COMMAND --help' for more information on a command.\n";
}

void Frontend::run_program(Options const& options)
{
    if (options.is_asking_for_help().first && !options.application().has_value()) {
        print_run_usage(options.short_executable_name());
    } else {
        const std::optional<std::string> application_opt = options.application();
        if (!application_opt.has_value()) {
            throw InvalidProgramArgumentsException(
                "Game or program not provided",
                Frontend::print_run_usage);
        }

        std::string const& program = application_opt.value();
        if (!is_supporting(program)) {
            throw InvalidProgramArgumentsException(
                fmt::format("Unsupported game or program: {}", program),
                Frontend::print_run_usage);
        }

        if (options.is_asking_for_help().first) {
            s_program_usages.at(program)(options.short_executable_name());
        } else {
            choose_emulator(program, options)->new_session()->run();
        }
    }
}

void Frontend::disassemble(Options const& options)
{
    using emu::memory::EmulatorMemory;
    using emu::util::file::read_file_into_vector;

    if (options.is_asking_for_help().first) {
        print_disassemble_usage(options.short_executable_name());
    } else {
        std::unordered_map<std::string, std::vector<std::string>> opts = options.options();
        if (!opts.contains("cpu")) {
            throw InvalidProgramArgumentsException(
                "No CPUs provided",
                Frontend::print_disassemble_usage);
        }
        std::vector<std::string> const& cpus = opts["cpu"];
        if (cpus.empty()) {
            throw InvalidProgramArgumentsException(
                "CPU has to be provided on the following format: --cpu=<CPU>",
                Frontend::print_disassemble_usage);
        } else if (cpus.size() > 1) {
            throw InvalidProgramArgumentsException(
                "Only one CPU can be provided at a time",
                Frontend::print_disassemble_usage);
        }
        std::string const& cpu = cpus[0];
        if (!options.path().has_value()) {
            throw InvalidProgramArgumentsException(
                "Path to a file has to be provided",
                Frontend::print_disassemble_usage);
        }

        const std::string file_path = options.path().value();

        if (cpu == "8080") {
            EmulatorMemory<u16, u8> memory;
            memory.add(read_file_into_vector(file_path));

            i8080::Disassembler disassembler(memory, std::cout);
            disassembler.disassemble();
        } else if (cpu == "Z80") {
            EmulatorMemory<u16, u8> memory;
            memory.add(read_file_into_vector(file_path));

            z80::Disassembler disassembler(memory, std::cout);
            disassembler.disassemble();
        } else {
            throw InvalidProgramArgumentsException(
                fmt::format("Invalid CPU: {}", cpu),
                Frontend::print_disassemble_usage);
        }
    }
}

void Frontend::test(Options const& options)
{
    if (options.is_asking_for_help().first) {
        print_test_usage(options.short_executable_name());
    } else {
        doctest::Context context;
        context.addFilter("test-case", "crosscutting*");
        std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

        if (!opts.contains("cpu")) {
            context.addFilter("test-case", "8080*");
            context.addFilter("test-case", "Z80*");
            context.addFilter("test-case", "LMC*");
        } else {
            for (std::string& cpu : opts["cpu"]) {
                context.addFilter("test-case", fmt::format("{}*", cpu).c_str());
            }
        }

        int const res = context.run();

        if (context.shouldExit()) {
            exit(res);
        }

        exit(res);
    }
}

void Frontend::print_run_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run APPLICATION [FLAGS]\n\n";
    std::cout << "Run an application\n\n";

    std::cout << "Applications:\n";

    for (auto& program_description : s_supported_programs) {
        std::string padding = create_padding(program_description.first.size(), s_padding_to_description);
        if (program_description.first == "NEWLINE") {
            std::cout << "\n";
        } else {
            std::cout << "  " << program_description.first << padding << program_description.second << "\n";
        }
    }

    std::cout << "\n\nRun './" << program_name
              << " run APPLICATION --help' for more information about running a specific application.\n";
}

void Frontend::print_disassemble_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " disassemble --cpu=<CPU> FILE_PATH\n\n";
    std::cout << "Disassemble a file for a given CPU\n\n";

    std::cout << "CPUs:\n";

    for (auto& cpu_description : s_supported_cpus) {
        std::string padding = create_padding(cpu_description.first.size(), s_padding_to_description);
        if (cpu_description.first == "NEWLINE") {
            std::cout << "\n";
        } else {
            std::cout << "  " << cpu_description.first << padding << cpu_description.second << "\n";
        }
    }
}

void Frontend::print_test_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " test [CPUs]\n\n";
    std::cout << "Run unit tests for one or more CPUs\n\n";

    std::cout << "CPUs:\n";

    for (auto& cpu_description : s_supported_cpus) {
        std::string padding = create_padding(cpu_description.first.size(), s_padding_to_description);
        if (cpu_description.first == "NEWLINE") {
            std::cout << "\n";
        } else {
            std::cout << "  " << cpu_description.first << padding << cpu_description.second << "\n";
        }
    }

    std::cout << "\n\nOne or multiple CPUs can be specified. The unit tests for all CPUs are run if no "
                 "CPU is provided.\nThe crosscutting unit tests are always run.\n";
    std::cout << "\nExamples:\n";

    for (auto& example_description : s_test_examples) {
        std::cout << "  " << example_description.second << ":\n";
        std::cout << "    "
                  << "./" << program_name << " test " << example_description.first << "\n\n";
    }
}

std::unique_ptr<Emulator> Frontend::choose_emulator(std::string const& program, Options const& options)
{
    using namespace applications;

    if (program == "pacman") {
        return std::make_unique<pacman::Pacman>(
            pacman::Settings::from_options(options),
            options.gui_type(pacman::print_usage));
    } else if (program == "prelim") {
        return std::make_unique<cpm::z80::CpmApplication>("roms/z80/prelim.com");
    } else if (program == "zexall") {
        return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexall.cim");
    } else if (program == "zexdoc") {
        return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexdoc.cim");
    } else if (program == "space_invaders") {
        return std::make_unique<space_invaders::SpaceInvaders>(
            space_invaders::Settings::from_options(options),
            options.gui_type(space_invaders::print_usage));
    } else if (program == "TST8080") {
        return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/TST8080.COM");
    } else if (program == "8080PRE") {
        return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/8080PRE.COM");
    } else if (program == "8080EXM") {
        return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/8080EXM.COM");
    } else if (program == "CPUTEST") {
        return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/CPUTEST.COM");
    } else if (program == "lmc_application") {
        if (options.path().has_value()) {
            return std::make_unique<lmc::LmcApplication>(
                options.path().value(),
                options.gui_type(lmc_application::print_usage));
        } else {
            throw InvalidProgramArgumentsException(
                "You have to specify the path of the file to run",
                lmc_application::print_usage);
        }
    } else {
        throw InvalidProgramArgumentsException(
            "Illegal program argument when choosing emulator",
            Frontend::print_run_usage);
    }
}

bool Frontend::is_supporting(std::string const& program)
{
    std::vector<std::string> program_names;
    std::transform(
        s_supported_programs.begin(),
        s_supported_programs.end(),
        std::back_inserter(program_names),
        [](std::pair<std::string, std::string> const& program_description) { return program_description.first; });
    std::vector<std::string> program_names_filtered;
    std::copy_if(
        program_names.begin(),
        program_names.end(),
        std::back_inserter(program_names_filtered),
        [](std::string const& program_name) { return program_name != "NEWLINE"; });

    return std::find(program_names_filtered.begin(), program_names_filtered.end(), program) != program_names_filtered.end();
}
}
