#ifndef MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_TOKEN_KIND_H
#define MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_TOKEN_KIND_H

#include <string>
#include <unordered_map>

namespace emu::applications::command_line_arguments {

    enum class TokenKind {
        LongOption,
        ShortOption,
        Equals,
        Identifier,
        Eof
    };

    static const inline std::unordered_map<TokenKind, std::string> TokenKind_as_string = {
            {TokenKind::LongOption, "LongOption"},
            {TokenKind::Equals, "Equals"},
            {TokenKind::ShortOption, "ShortOption"},
            {TokenKind::Identifier, "Identifier"},
            {TokenKind::Eof, "Eof"},
    };
}

#endif //MIKA_EMULATORS_APPLICATIONS_COMMAND_LINE_ARGUMENTS_TOKEN_KIND_H
