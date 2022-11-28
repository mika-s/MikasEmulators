#include "lmc_integer.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"

namespace emu::lmc {

    LmcInteger::LmcInteger(int literal)
        : m_literal(literal) {
    }

    Data LmcInteger::eval() {
        return Data(m_literal);
    }

    LmcInteger LmcInteger::parse(Scanner &scanner) {
        int literal = scanner.current_token().int_literal();
        scanner.skip(TokenKind::Integer);

        return LmcInteger(literal);
    }
}
