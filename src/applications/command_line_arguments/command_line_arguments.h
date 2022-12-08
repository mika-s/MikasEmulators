#pragma once

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
