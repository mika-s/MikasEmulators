#define DOCTEST_CONFIG_IMPLEMENT

#include <iostream>
#include <unordered_map>
#include "doctest.h"
#include "8080/frontend.h"
#include "z80/frontend.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"

using emu::exceptions::InvalidProgramArgumentsException;
using emu::util::string::find_short_executable_name;

void print_usage(const std::string &program_name) {
    std::cout << "USAGE:\n\n";
    std::cout << "CPU:\n";
    std::cout << "  - 8080: Emulates the Intel 8080 CPU.\n\n";
    std::cout << "       Modes:\n";
    std::cout << "       - disassemble: Shows the assembly code of the given file.\n";
    std::cout << "         Example: ./" << program_name << " 8080 disassemble my_file.h\n";
    std::cout << "       - run: Runs a specific game or program. Available: "
              << emu::i8080::Frontend::supported() << "\n";
    std::cout << "       - test: Run unit tests related to 8080.\n\n";
    std::cout << "       Example: ./" << program_name << " 8080 run space_invaders\n";
    std::cout << "\n\n";
    std::cout << "  - Z80: Emulates the Z80 CPU.\n\n";
    std::cout << "       Modes:\n";
    std::cout << "       - disassemble: Shows the assembly code of the given file.\n";
    std::cout << "         Example: ./" << program_name << " Z80 disassemble my_file.h\n";
    std::cout << "       - run: Runs a specific game or program. Available: "
              << emu::z80::Frontend::supported() << "\n";
    std::cout << "       - test: Run unit tests related to Z80.\n\n";
    std::cout << "       Example: ./" << program_name << " Z80 run pacman\n";
}

std::unordered_map<std::string, std::vector<std::string>> find_options(std::vector<std::string> args) {
    const std::string dipswitch_flag = "-d";
    const std::string gui_flag = "-g";

    std::unordered_map<std::string, std::vector<std::string>> options;

    for (std::size_t arg_idx = 0; arg_idx < args.size(); ++arg_idx) {
        if (args[arg_idx] == dipswitch_flag) {
            if (arg_idx == args.size() - 1) {
                throw InvalidProgramArgumentsException(
                        dipswitch_flag + " flag at the end of the line, without a value."
                );
            }

            if (options.count(dipswitch_flag) == 0) {
                std::vector<std::string> vec;
                vec.emplace_back(args[arg_idx + 1]);
                options[dipswitch_flag] = vec;
            } else {
                options[dipswitch_flag].push_back(args[arg_idx + 1]);
            }
        } else if (args[arg_idx] == gui_flag) {
            if (arg_idx == args.size() - 1) {
                throw InvalidProgramArgumentsException(
                        gui_flag + " flag at the end of the line, without a value."
                );
            }

            if (options.count(gui_flag) == 0) {
                std::vector<std::string> vec;
                vec.emplace_back(args[arg_idx + 1]);
                options[gui_flag] = vec;
            } else {
                options[gui_flag].push_back(args[arg_idx + 1]);
            }
        }
    }

    return options;
}

// NOLINTNEXTLINE(modernize-avoid-c-arrays)
std::vector<std::string> argv_to_vector(int argc, char *argv[]) {
    std::vector<std::string> args;

    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    return args;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args = argv_to_vector(argc, argv);
    const std::string short_name = find_short_executable_name(argv[0]);

    try {
        if (argc > 1) {
            std::unordered_map<std::string, std::vector<std::string>> options = find_options(args);
            const std::string cpu = args[1];

            if (cpu == "8080") {
                emu::i8080::Frontend::run(args, options);
            } else if (cpu == "Z80") {
                emu::z80::Frontend::run(args, options);
            } else {
                throw InvalidProgramArgumentsException("Unknown CPU");
            }
        } else {
            throw InvalidProgramArgumentsException("No arguments provided");
        }
    } catch (InvalidProgramArgumentsException &ex) {
        std::cout << ex.what() << "\n\n";
        print_usage(short_name);
    }

    return 0;
}
