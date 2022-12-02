#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_COMMAND_LINE_ARGUMENTS_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_COMMAND_LINE_ARGUMENTS_H

#include "applications/options.h"
#include <string>
#include <vector>

namespace emu::applications::command_line_arguments {

    class CommandLineArguments {
    public:
        static Options find_options(std::vector<std::string> args);

    private:
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_COMMAND_LINE_ARGUMENTS_H
