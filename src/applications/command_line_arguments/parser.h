#pragma once

namespace emu::applications {
class Options;
}
namespace emu::applications::command_line_arguments {
class Scanner;
}

namespace emu::applications::command_line_arguments {

enum class ParserStates {
    Command,
    Application,
    MaybeOptions,
    MaybePath,
    Finished,
};

class Parser {
public:
    Parser();

    void parse(Scanner& scanner, Options& options);

private:
    ParserStates m_state;
};
}
