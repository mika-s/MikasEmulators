#include "identifier.h"
#include "command_line_arguments/scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"

namespace emu::applications::command_line_arguments {

    std::string Identifier::parse(Scanner &scanner) {
        std::string literal = scanner.current_token().string_literal();

        scanner.skip(TokenKind::Identifier);

        return literal;
    }
}
