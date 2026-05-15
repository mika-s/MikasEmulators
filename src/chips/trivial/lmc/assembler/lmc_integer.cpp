#include "lmc_integer.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"

namespace emu::lmc {

LmcInteger::LmcInteger(const int literal)
    : m_literal(literal)
{
}

auto LmcInteger::eval() const -> Data
{
    return Data(m_literal);
}

auto LmcInteger::parse(Scanner& scanner) -> LmcInteger
{
    int const literal = scanner.current_token().int_literal();
    scanner.skip(TokenKind::Integer);

    return LmcInteger(literal);
}
}
