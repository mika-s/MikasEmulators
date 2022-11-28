#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_TOKEN_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_TOKEN_H

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

        [[nodiscard]] TokenKind kind() const;

        [[nodiscard]] int int_literal() const;

        [[nodiscard]] std::string label_literal() const;

        friend std::ostream &operator<<(std::ostream &os, const Token &rhs) {
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

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_TOKEN_H
