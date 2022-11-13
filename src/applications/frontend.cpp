#include "frontend.h"
#include "applications/cpm_8080/cpm_application.h"
#include "applications/cpm_z80/cpm_application.h"
#include "applications/pacman/pacman.h"
#include "applications/pacman/settings.h"
#include "applications/space_invaders/settings.h"
#include "applications/space_invaders/space_invaders.h"
#include "chips/8080/disassembler8080.h"
#include "chips/z80/disassemblerZ80.h"
#include "cpm_8080/usage.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/memory/emulator_memory.h"
#include "crosscutting/misc/emulator.h"
#include "crosscutting/misc/session.h"
#include "crosscutting/util/file_util.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include "options.h"
#include "pacman/usage.h"
#include "space_invaders/usage.h"
#include <algorithm>
#include <cstdlib>
#include <ext/alloc_traits.h>
#include <fmt/core.h>
#include <iostream>
#include <iterator>

namespace emu::applications {

    using emu::exceptions::InvalidProgramArgumentsException;
    using emu::util::string::create_padding;

    void Frontend::run(Options &options) {
        const std::vector<std::string> args = options.args();
        if (args.size() < global_minimum_number_of_arguments) {
            throw InvalidProgramArgumentsException(
                    "Wrong number of arguments.",
                    Frontend::print_main_usage
            );
        }

        const std::string &command = args[command_argument];

        if (command == "disassemble") {
            disassemble(args, options);
        } else if (command == "run") {
            run(args, options);
        } else if (command == "test") {
            test(args, options);
        } else {
            throw InvalidProgramArgumentsException(
                    fmt::format("Unknown command: {}", command),
                    Frontend::print_main_usage
            );
        }
    }

    void Frontend::print_main_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " COMMAND [ARGUMENTS] [FLAGS]\n\n";
        std::cout << "A collection of emulators\n\n";

        std::cout << "Commands:\n";

        for (auto &command_description: command_descriptions) {
            std::string padding = create_padding(command_description.first.size(), padding_to_description);
            std::cout << "  " << command_description.first << padding << command_description.second << "\n";
        }

        std::cout << "\n\nRun './" << program_name << " COMMAND --help' for more information on a command.\n";
    }

    void Frontend::run(const std::vector<std::string> &args, Options &options) {
        if (args.size() < run_minimum_number_of_arguments) {
            throw InvalidProgramArgumentsException(
                    fmt::format(
                            "Wrong number of arguments to run mode: {}. Minimum: {}",
                            args.size(),
                            run_minimum_number_of_arguments
                    ),
                    Frontend::print_run_usage
            );
        }

        if (options.is_asking_for_help() && args.size() == run_minimum_number_of_arguments) {
            print_run_usage(options.short_executable_name());
        } else {
            const std::string &program = args[run_program_argument];
            if (!is_supporting(program)) {
                throw InvalidProgramArgumentsException(
                        fmt::format("Unsupported game or program: {}", program),
                        Frontend::print_run_usage
                );
            }

            if (options.is_asking_for_help()) {
                program_usages.at(program)(options.short_executable_name());
            } else {
                choose_emulator(program, options)->new_session()->run();
            }
        }
    }

    void Frontend::disassemble(const std::vector<std::string> &args, Options &options) {
        using emu::i8080::Disassembler8080;
        using emu::util::file::read_file_into_vector;
        using emu::z80::DisassemblerZ80;

        if (options.is_asking_for_help()) {
            print_disassemble_usage(options.short_executable_name());
        } else if (args.size() < disassembly_minimum_number_of_arguments) {
            throw InvalidProgramArgumentsException(
                    fmt::format(
                            "Wrong number of arguments to disassemble mode: {}. Minimum: {}",
                            args.size(),
                            disassembly_minimum_number_of_arguments
                    ),
                    Frontend::print_disassemble_usage
            );
        } else {
            const std::string &cpu = args[disassembly_cpu_argument];
            const std::string &file_path = args[disassembly_file_path_argument];

            if (cpu == "8080") {
                i8080::EmulatorMemory memory;
                memory.add(read_file_into_vector(file_path));

                Disassembler8080 disassembler(memory, std::cout);
                disassembler.disassemble();
            } else if (cpu == "Z80") {
                z80::EmulatorMemory memory;
                memory.add(read_file_into_vector(file_path));

                DisassemblerZ80 disassembler(memory, std::cout);
                disassembler.disassemble();
            } else {
                throw InvalidProgramArgumentsException(
                        fmt::format("Invalid CPU: {}", cpu),
                        Frontend::print_disassemble_usage
                );
            }
        }
    }

    void Frontend::test(const std::vector<std::string> &args, Options &options) {
        if (args.size() < test_minimum_number_of_arguments) {
            throw InvalidProgramArgumentsException(
                    fmt::format(
                            "Wrong number of arguments to test mode: {}. Minimum: {}",
                            args.size(),
                            test_minimum_number_of_arguments
                    ),
                    Frontend::print_test_usage
            );
        }

        if (options.is_asking_for_help()) {
            print_test_usage(options.short_executable_name());
        } else {
            doctest::Context context;
            context.addFilter("test-case", "crosscutting*");

            if (args.size() == test_number_of_arguments_when_no_cpus_provided) {
                context.addFilter("test-case", "8080*");
                context.addFilter("test-case", "Z80*");
            } else {
                for (std::size_t arg_idx = 1; arg_idx < args.size(); ++arg_idx) {
                    const std::string cpu = args[arg_idx] + "*";
                    context.addFilter("test-case", cpu.c_str());
                }
            }

            const int res = context.run();

            if (context.shouldExit()) {
                exit(res);
            }

            exit(res);
        }
    }

    void Frontend::print_run_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " run APPLICATION [FLAGS]\n\n";
        std::cout << "Run an application\n\n";

        std::cout << "Applications:\n";

        for (auto &program_description: supported_programs) {
            std::string padding = create_padding(program_description.first.size(), padding_to_description);
            if (program_description.first == "NEWLINE") {
                std::cout << "\n";
            } else {
                std::cout << "  " << program_description.first << padding << program_description.second << "\n";
            }
        }

        std::cout << "\n\nRun './" << program_name
                  << " run APPLICATION --help' for more information about running a specific application.\n";
    }

    void Frontend::print_disassemble_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " disassemble CPU FILE_PATH\n\n";
        std::cout << "Disassemble a file for a given CPU\n\n";

        std::cout << "CPUs:\n";

        for (auto &cpu_description: supported_cpus) {
            std::string padding = create_padding(cpu_description.first.size(), padding_to_description);
            if (cpu_description.first == "NEWLINE") {
                std::cout << "\n";
            } else {
                std::cout << "  " << cpu_description.first << padding << cpu_description.second << "\n";
            }
        }
    }

    void Frontend::print_test_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " test [CPUs]\n\n";
        std::cout << "Run unit tests for one or more CPUs\n\n";

        std::cout << "CPUs:\n";

        for (auto &cpu_description: supported_cpus) {
            std::string padding = create_padding(cpu_description.first.size(), padding_to_description);
            if (cpu_description.first == "NEWLINE") {
                std::cout << "\n";
            } else {
                std::cout << "  " << cpu_description.first << padding << cpu_description.second << "\n";
            }
        }

        std::cout << "\n\nOne or multiple CPUs can be specified. The unit tests for all CPUs are run if no "
                     "CPU is provided.\nThe crosscutting unit tests are always run.\n";
        std::cout << "\nExamples:\n";

        for (auto &example_description: test_examples) {
            std::cout << "  " << example_description.second << ":\n";
            std::cout << "    "
                      << "./" << program_name << " run test " << example_description.first << "\n\n";
        }
    }

    std::unique_ptr<Emulator> Frontend::choose_emulator(const std::string &program, Options &options) {
        using namespace applications;

        if (program == "pacman") {
            return std::make_unique<pacman::Pacman>(
                    pacman::Settings::from_options(options),
                    options.gui_type(pacman::print_usage)
            );
        } else if (program == "prelim") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/prelim.com");
        } else if (program == "zexall") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexall.cim");
        } else if (program == "zexdoc") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexdoc.cim");
        } else if (program == "space_invaders") {
            return std::make_unique<space_invaders::SpaceInvaders>(
                    space_invaders::Settings::from_options(options),
                    options.gui_type(space_invaders::print_usage)
            );
        } else if (program == "TST8080") {
            return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/TST8080.COM");
        } else if (program == "8080PRE") {
            return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/8080PRE.COM");
        } else if (program == "8080EXM") {
            return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/8080EXM.COM");
        } else if (program == "CPUTEST") {
            return std::make_unique<cpm::i8080::CpmApplication>("roms/8080/CPUTEST.COM");
        } else {
            throw InvalidProgramArgumentsException(
                    "Illegal program argument when choosing emulator",
                    Frontend::print_run_usage
            );
        }
    }

    bool Frontend::is_supporting(const std::string &program) {
        std::vector<std::string> program_names;
        std::transform(
                supported_programs.begin(),
                supported_programs.end(),
                std::back_inserter(program_names),
                [](const std::pair<std::string, std::string> &program_description) { return program_description.first; }
        );
        std::vector<std::string> program_names_filtered;
        std::copy_if(
                program_names.begin(),
                program_names.end(),
                std::back_inserter(program_names_filtered),
                [](const std::string &program_name) { return program_name != "NEWLINE"; }
        );

        return std::find(program_names_filtered.begin(), program_names_filtered.end(), program) != program_names_filtered.end();
    }
}
