#pragma once

#include "token_kind.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace emu::applications::command_line_arguments {

class Token {
public:
    explicit Token(TokenKind kind);

    explicit Token(TokenKind kind, std::string literal);

    [[nodiscard]] TokenKind kind() const;

    [[nodiscard]] std::string string_literal() const;

    friend std::ostream& operator<<(std::ostream& os, Token const& rhs)
    {
        os << TokenKind_as_string.at(rhs.m_kind);
        if (rhs.m_kind == TokenKind::ShortOption) {
            os << "(" << rhs.m_string_literal << ")";
        } else if (rhs.m_kind == TokenKind::LongOption) {
            os << "(" << rhs.m_string_literal << ")";
        } else if (rhs.m_kind == TokenKind::Identifier) {
            os << "(" << rhs.m_string_literal << ")";
        }
        return os;
    }

private:
    TokenKind m_kind;
    std::string m_string_literal;
};
}
