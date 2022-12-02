#include "command_line_arguments.h"
#include "options.h"
#include "parser.h"
#include "scanner.h"

namespace emu::applications::command_line_arguments {

    Options CommandLineArguments::find_options(std::vector<std::string> args) {
        Scanner scanner({args.begin() + 1, args.end()});
        Options options(args);
        Parser parser;
        parser.parse(scanner, options);

        return options;
    }
}
