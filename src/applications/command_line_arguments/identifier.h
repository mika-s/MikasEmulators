#pragma once

#include <string>

namespace emu::applications::command_line_arguments {
class Scanner;
}

namespace emu::applications::command_line_arguments {

class Identifier {
public:
    static std::string parse(Scanner& scanner);
};
}
