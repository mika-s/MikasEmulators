#define DOCTEST_CONFIG_IMPLEMENT

#include <iostream>
#include "doctest.h"
#include "applications/frontend.h"
#include "applications/options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"

using emu::applications::Frontend;
using emu::applications::Options;
using emu::exceptions::InvalidProgramArgumentsException;
using emu::util::string::find_short_executable_name;

void print_usage(const std::string &program_name) {
    std::cout << "USAGE:\n\n";

    std::cout << "Run a specific game or program:\n";
    std::cout << "./" << program_name << " run <application> [flags]\n\n";
    std::cout << "Applications:\n";
    std::cout << Frontend::supported() << "\n\n";

    std::cout << "Show the assembly code of the given file:\n";
    std::cout << "./" << program_name << " disassemble <cpu> <file>\n\n";

    std::cout << "Run the unit tests of a given CPU:\n";
    std::cout << "./" << program_name << " test <cpu1>, <cpu2>, ..., <cpuN>\n";
    std::cout << "All tests are run if no CPUs are provided.\n\n";

    std::cout << "Examples:\n\n";
    std::cout << "./" << program_name << " run space_invaders -g debugging -d b=1000\n";
    std::cout << "Runs Space Invaders with the debugging GUI and b=1000 as manually set dipswitch.\n\n";
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
    try {
        if (argc > 1) {
            Options options(argv_to_vector(argc, argv));
            Frontend::run(options);
        } else {
            throw InvalidProgramArgumentsException("No arguments provided");
        }
    } catch (InvalidProgramArgumentsException &ex) {
        std::cout << ex.what() << "\n\n";
        print_usage(find_short_executable_name(argv[0]));
    }

    return 0;
}
