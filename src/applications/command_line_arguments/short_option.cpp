#include "short_option.h"
#include "command_line_arguments/scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"
#include "options.h"
#include <fmt/core.h>
#include <stdexcept>
#include <string>

namespace emu::applications::command_line_arguments {

    void ShortOption::parse(Scanner &scanner, Options &options) {
        const std::string name_literal = scanner.current_token().string_literal();
        scanner.skip(TokenKind::ShortOption);

        if (scanner.current_token().kind() == TokenKind::Identifier) {
            const std::string value_literal = scanner.current_token().string_literal();

            scanner.skip(TokenKind::Identifier);

            options.add_option(name_literal, value_literal);
        } else {
            throw std::invalid_argument(fmt::format("Option -{} requires a value after it", name_literal));
        }
    }
}
