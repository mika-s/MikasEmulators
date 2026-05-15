#include "token.h"
#include "token_kind.h"
#include <stdexcept>
#include <utility>

namespace emu::lmc {

Token::Token(const TokenKind kind)
    : m_kind(kind)
    , m_int_literal(0)
{
}

Token::Token(const TokenKind kind, const int literal)
    : m_kind(kind)
    , m_int_literal(literal)
{
    if (kind != TokenKind::Integer) {
        throw std::invalid_argument("TokenKind has to be Integer when passing integer literal");
    }
}

Token::Token(const TokenKind kind, std::string literal)
    : m_kind(kind)
    , m_int_literal(0)
    , m_label_literal(std::move(literal))
{
    if (kind != TokenKind::Label) {
        throw std::invalid_argument("TokenKind has to be Label when passing string literal");
    }
}

auto Token::kind() const -> TokenKind
{
    return m_kind;
}

auto Token::int_literal() const -> int
{
    return m_int_literal;
}

auto Token::label_literal() const -> std::string
{
    return m_label_literal;
}
}
