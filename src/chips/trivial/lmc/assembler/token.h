#pragma once

#include "token_kind.h"
#include <iostream>
#include <string>
#include <unordered_map>

namespace emu::lmc {

class Token {
public:
    explicit Token(TokenKind kind);

    explicit Token(TokenKind kind, int literal);

    explicit Token(TokenKind kind, std::string literal);

    [[nodiscard]] auto kind() const -> TokenKind;

    [[nodiscard]] auto int_literal() const -> int;

    [[nodiscard]] auto label_literal() const -> std::string;

    friend auto operator<<(std::ostream& os, Token const& rhs) -> std::ostream& // NOLINT(*-identifier-length)
    {
        os << TokenKind_as_string.at(rhs.m_kind);
        if (rhs.m_kind == TokenKind::Integer) {
            os << "(" << std::to_string(rhs.m_int_literal) << ")";
        } else if (rhs.m_kind == TokenKind::Label) {
            os << "(\"" << rhs.m_label_literal << "\")";
        }
        return os;
    }

private:
    TokenKind m_kind;
    int m_int_literal;
    std::string m_label_literal;
};
}
