#include <iostream>
#include <unordered_map>
#include "disassembler8080.h"
#include "frontend.h"
#include "invalid_program_arguments_exception.h"
#include "string_util.h"

using emu::util::exceptions::InvalidProgramArgumentsException;

void print_usage(const std::string &program_name) {
    std::cout << "USAGE:\n\n";
    std::cout << "CPU:\n";
    std::cout << "  - 8080: Emulates the Intel 8080 CPU.\n\n";
    std::cout << "    Modes:\n";
    std::cout << "    - disassemble: Shows the assembly code of the given file.\n";
    std::cout << "      Example: ./" << program_name << " 8080 disassemble my_file.h\n";
    std::cout << "    - run: Runs a specific game or program. Available: "
              << emu::cpu8080::Frontend::supported() << "\n";
    std::cout << "      Example: ./" << program_name << " 8080 run space_invaders\n";
}

std::unordered_map<std::string, std::vector<std::string>> find_options(int argc, char *argv[]) {
    std::unordered_map<std::string, std::vector<std::string>> options;

    for (int arg_idx = 0; arg_idx < argc; ++arg_idx) {
        if (std::string(argv[arg_idx]) == "-d") {
            if (arg_idx != argc - 1) {
                if (options.count("-d") == 0) {
                    std::vector<std::string> vec;
                    vec.emplace_back(argv[arg_idx + 1]);
                    options["-d"] = vec;
                } else {
                    options["-d"].push_back(argv[arg_idx + 1]);
                }
            } else {
                throw InvalidProgramArgumentsException("-d flag at the end of the line, without a value.");
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
    }
    catch (InvalidProgramArgumentsException &ex) {
        std::cout << ex.what() << "\n\n";
        print_usage(short_name);
    }

    return 0;
}
