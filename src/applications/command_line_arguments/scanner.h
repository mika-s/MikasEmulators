#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SCANNER_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SCANNER_H

#include "token.h"
#include "token_kind.h"
#include <cstddef>
#include <string>
#include <vector>

namespace emu::applications::command_line_arguments {

    class Scanner {
    public:
        explicit Scanner(std::vector<std::string> args);

        Token current_token();

        void skip(TokenKind next);

    private:
        std::vector<std::string> m_args;
        std::vector<Token> tokens;
        std::size_t m_current_pos = 0;
        std::size_t m_arg_idx = 0;

        void read_next_token();

        void read_next_arg();

        void read_token(const std::string &line);

        bool handle_single_character(const std::string &line);

        bool handle_short_option(const std::string &line);

        bool handle_long_option(const std::string &line);

        bool handle_identifier(const std::string &line);
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_SCANNER_H
