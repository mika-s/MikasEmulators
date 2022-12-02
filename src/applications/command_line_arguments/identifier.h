#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_IDENTIFIER_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_IDENTIFIER_H

#include <string>

namespace emu::applications::command_line_arguments {
    class Scanner;
}

namespace emu::applications::command_line_arguments {

    class Identifier {
    public:
        static std::string parse(Scanner &scanner);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_IDENTIFIER_H
