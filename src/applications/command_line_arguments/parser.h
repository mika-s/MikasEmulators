#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_PARSER_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_PARSER_H

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

        void parse(Scanner &scanner, Options &options);

    private:
        [[maybe_unused]] ParserStates m_state;
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_PARSER_H
