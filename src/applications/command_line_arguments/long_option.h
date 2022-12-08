#pragma once

namespace emu::applications {
class Options;
}
namespace emu::applications::command_line_arguments {
class Scanner;
}

namespace emu::applications::command_line_arguments {

class LongOption {
public:
    static void parse(Scanner& scanner, Options& options);
};
}
