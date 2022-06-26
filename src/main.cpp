#define DOCTEST_CONFIG_IMPLEMENT

#include <iostream>
#include <unordered_map>
#include "doctest.h"
#include "8080/disassembler8080.h"
#include "8080/frontend.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"

using emu::exceptions::InvalidProgramArgumentsException;

void print_usage(const std::string &program_name) {
    std::cout << "USAGE:\n\n";
    std::cout << "CPU:\n";
    std::cout << "  - 8080: Emulates the Intel 8080 CPU.\n\n";
    std::cout << "    Modes:\n";
    std::cout << "    - disassemble: Shows the assembly code of the given file.\n";
    std::cout << "      Example: ./" << program_name << " 8080 disassemble my_file.h\n";
    std::cout << "    - run: Runs a specific game or program. Available: "
              << emu::cpu8080::Frontend::supported() << "\n";
    std::cout << "    - test: Run unit tests related to 8080.";
    std::cout << "      Example: ./" << program_name << " 8080 run space_invaders\n";
}

std::unordered_map<std::string, std::vector<std::string>> find_options(int argc, char *argv[]) {
    const std::string dipswitch_flag = "-d";
    const std::string gui_flag = "-g";

    std::unordered_map<std::string, std::vector<std::string>> options;

    for (int arg_idx = 0; arg_idx < argc; ++arg_idx) {
        if (std::string(argv[arg_idx]) == dipswitch_flag) {
            if (arg_idx == argc - 1) {
                throw InvalidProgramArgumentsException(dipswitch_flag + " flag at the end of the line, without a value.");
            }

            if (options.count(dipswitch_flag) == 0) {
                std::vector<std::string> vec;
                vec.emplace_back(argv[arg_idx + 1]);
                options[dipswitch_flag] = vec;
            } else {
                options[dipswitch_flag].push_back(argv[arg_idx + 1]);
            }
        } else if (std::string(argv[arg_idx]) == gui_flag) {
            if (arg_idx == argc - 1) {
                throw InvalidProgramArgumentsException(gui_flag + " flag at the end of the line, without a value.");
            }

            if (options.count(gui_flag) == 0) {
                std::vector<std::string> vec;
                vec.emplace_back(argv[arg_idx + 1]);
                options[gui_flag] = vec;
            } else {
                options[gui_flag].push_back(argv[arg_idx + 1]);
            }
        }
    }

    return options;
}

int main(int argc, char *argv[]) {
    const std::string short_name = emu::util::string::find_short_executable_name(argv[0]);

    try {
        if (argc > 1) {
            const std::string cpu(argv[1]);
            std::unordered_map<std::string, std::vector<std::string>> options = find_options(argc, argv);

            if (cpu == "8080") {
                emu::cpu8080::Frontend::run(argc, argv, options);
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
