#define DOCTEST_CONFIG_IMPLEMENT

#include "applications/command_line_arguments/command_line_arguments.h"
#include "applications/frontend.h"
#include "applications/options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/exceptions/rom_file_not_found_exception.h"
#include "crosscutting/util/string_util.h"
#include "doctest.h"
#include <SDL.h>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using emu::applications::Frontend;
using emu::applications::Options;
using emu::applications::command_line_arguments::CommandLineArguments;
using emu::exceptions::InvalidProgramArgumentsException;
using emu::exceptions::RomFileNotFoundException;
using emu::util::string::find_short_executable_name;

int main(int argc, char* argv[])
{
    const std::string short_executable_name = find_short_executable_name(std::string(argv[0]));
    try {
        if (argc > 1) {
            const std::vector<std::string> args(argv, argv + argc);
            const Options options = CommandLineArguments::find_options(args);

            if (options.is_failed().first && !options.is_asking_for_help().first) {
                throw InvalidProgramArgumentsException(options.is_failed().second, Frontend::print_main_usage);
            } else if (options.is_debugging_cmd_parser().first) {
                for (std::string const& token : options.is_debugging_cmd_parser().second) {
                    std::cout << token << " ";
                }
                std::cout << "\n";
                return 3;
            }

            Frontend::run(options);
            SDL_Quit();
        } else {
            Frontend::print_main_usage(short_executable_name);
        }
    } catch (InvalidProgramArgumentsException& ex) {
        std::cout << ex.what() << "\n";
        ex.usage_function()(short_executable_name);
        return 1;
    } catch (RomFileNotFoundException& ex) {
        std::cout << ex.what() << "\n";
        SDL_Quit();
        return 2;
    }

    return 0;
}
