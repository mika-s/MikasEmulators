#include "token.h"
#include "token_kind.h"
#include <stdexcept>
#include <utility>

namespace emu::applications::command_line_arguments {

Token::Token(const TokenKind kind)
    : m_kind(kind)
{
}

Token::Token(const TokenKind kind, std::string literal)
    : m_kind(kind)
    , m_string_literal(std::move(literal))
{
    if (kind != TokenKind::ShortOption && kind != TokenKind::LongOption && kind != TokenKind::Identifier) {
        throw std::invalid_argument(
            "TokenKind has to be ShortOption, LongOption or Identifier when passing string literal");
    }
}

auto Token::kind() const -> TokenKind
{
    return m_kind;
}

auto Token::string_literal() const -> std::string
{
    return m_string_literal;
}
}
