#pragma once

#include <string>

namespace emu::applications::command_line_arguments {
class Scanner;
}

namespace emu::applications::command_line_arguments {

class Identifier {
public:
    static auto parse(Scanner& scanner) -> std::string;
};
}
