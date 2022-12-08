#include "token.h"
#include "token_kind.h"
#include <stdexcept>
#include <utility>

namespace emu::applications::command_line_arguments {

Token::Token(TokenKind kind)
    : m_kind(kind)
    , m_string_literal("")
{
}

Token::Token(TokenKind kind, std::string literal)
    : m_kind(kind)
    , m_string_literal(std::move(literal))
{
    if (kind != TokenKind::ShortOption && kind != TokenKind::LongOption && kind != TokenKind::Identifier) {
        throw std::invalid_argument(
            "TokenKind has to be ShortOption, LongOption or Identifier when passing string literal");
    }
}

TokenKind Token::kind() const
{
    return m_kind;
}

std::string Token::string_literal() const
{
    return m_string_literal;
}
}
