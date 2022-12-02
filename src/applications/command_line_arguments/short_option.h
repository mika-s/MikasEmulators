#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SHORT_OPTION_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SHORT_OPTION_H

namespace emu::applications {
    class Options;
}
namespace emu::applications::command_line_arguments {
    class Scanner;
}

namespace emu::applications::command_line_arguments {

    class ShortOption {
    public:
        static void parse(Scanner &scanner, Options &options);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SHORT_OPTION_H
