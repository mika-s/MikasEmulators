#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <sstream>
#include "doctest.h"
#include "frontend.h"
#include "applications/cpm_8080/cpm_application.h"
#include "applications/cpm_z80/cpm_application.h"
#include "applications/pacman/pacman.h"
#include "applications/space_invaders/space_invaders.h"
#include "chips/8080/disassembler8080.h"
#include "chips/8080/emulator_memory.h"
#include "chips/z80/disassemblerZ80.h"
#include "chips/z80/emulator_memory.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/file_util.h"

namespace emu::applications {

    using emu::exceptions::InvalidProgramArgumentsException;

    void Frontend::run(Options &options) {
        const std::vector<std::string> args = options.args();
        if (args.size() >= global_minimum_number_of_arguments) {
            const std::string &mode = args[mode_argument];

            if (mode == "disassemble") {
                disassemble(args);
            } else if (mode == "run") {
                run(args, options);
            } else if (mode == "test") {
                test(args);
            } else {
                throw InvalidProgramArgumentsException("Unknown mode: " + mode + "\n");
            }
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments.");
        }
    }

    std::string Frontend::supported() {
        const char *const delimiter = ", ";

        std::ostringstream imploded;
        std::copy(supported_programs.begin(), supported_programs.end(),
                  std::experimental::ostream_joiner<std::string>(imploded, delimiter));

        return imploded.str();
    }

    void Frontend::run(const std::vector<std::string> &args, Options &options) {
        if (args.size() >= run_minimum_number_of_arguments) {
            const std::string &program = args[run_program_argument];
            if (is_supporting(program)) {
                choose_emulator(program, options)->new_session()->run();
            } else {
                throw InvalidProgramArgumentsException("Unsupported game or program.");
            }
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments to run mode.");
        }
    }

    void Frontend::disassemble(const std::vector<std::string> &args) {
        using emu::i8080::Disassembler8080;
        using emu::z80::DisassemblerZ80;
        using emu::util::file::read_file_into_vector;

        if (args.size() == disassembly_number_of_arguments) {
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
                throw InvalidProgramArgumentsException("Invalid CPU type.");
            }
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments to disassemble mode.");
        }
    }

    std::unique_ptr<Emulator> Frontend::choose_emulator(const std::string &program, Options &options) {
        using namespace applications;

        if (program == "pacman") {
            return std::make_unique<pacman::Pacman>(options.pacman_settings(), options.gui_type());
        } else if (program == "prelim") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/prelim.com");
        } else if (program == "zexall") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexall.cim");
        } else if (program == "zexdoc") {
            return std::make_unique<cpm::z80::CpmApplication>("roms/z80/zexdoc.cim");
        } else if (program == "space_invaders") {
            return std::make_unique<space_invaders::SpaceInvaders>(
                    options.space_invaders_settings(), options.gui_type()
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
            throw InvalidProgramArgumentsException("Illegal program argument when choosing emulator");
        }
    }

    void Frontend::test(const std::vector<std::string> &args) {
        if (args.size() >= test_minimum_number_of_arguments) {
            doctest::Context context;
            context.addFilter("test-case", "crosscutting*");

            if (args.size() == test_number_of_arguments_when_no_cpus_provided) {
                context.addFilter("test-case", "8080*");
                context.addFilter("test-case", "Z80*");
            } else {
                for (size_t arg_idx = 1; arg_idx < args.size(); ++arg_idx) {
                    const std::string cpu = args[arg_idx] + "*";
                    context.addFilter("test-case", cpu.c_str());
                }
            }

            int res = context.run();

            if (context.shouldExit()) {
                exit(res);
            }

            exit(res);
        } else {
            throw InvalidProgramArgumentsException("Wrong number of arguments to test mode.");
        }
    }

    bool Frontend::is_supporting(const std::string &program) {
        return std::find(supported_programs.begin(), supported_programs.end(), program) != supported_programs.end();
    }
}
