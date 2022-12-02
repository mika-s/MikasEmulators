#include "long_option.h"
#include "command_line_arguments/scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"
#include "options.h"
#include <string>

namespace emu::applications::command_line_arguments {

    void LongOption::parse(Scanner &scanner, Options &options) {
        const std::string name_literal = scanner.current_token().string_literal();
        scanner.skip(TokenKind::LongOption);

        if (scanner.current_token().kind() == TokenKind::Equals) {
            scanner.skip(TokenKind::Equals);

            const std::string value_literal = scanner.current_token().string_literal();

            scanner.skip(TokenKind::Identifier);

            options.add_option(name_literal, value_literal);
        } else {
            options.add_option(name_literal);
        }
    }
}
