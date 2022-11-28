#include "token.h"
#include "token_kind.h"
#include <stdexcept>
#include <utility>

namespace emu::lmc {

    Token::Token(TokenKind kind)
        : m_kind(kind),
          m_int_literal(0),
          m_label_literal("") {
    }

    Token::Token(TokenKind kind, int literal)
        : m_kind(kind),
          m_int_literal(literal),
          m_label_literal("") {
        if (kind != TokenKind::Integer) {
            throw std::invalid_argument("TokenKind has to be Integer when passing integer literal");
        }
    }

    Token::Token(TokenKind kind, std::string literal)
        : m_kind(kind),
          m_int_literal(0),
          m_label_literal(std::move(literal)) {
        if (kind != TokenKind::Label) {
            throw std::invalid_argument("TokenKind has to be Label when passing string literal");
        }
    }

    TokenKind Token::kind() const {
        return m_kind;
    }

    int Token::int_literal() const {
        return m_int_literal;
    }

    std::string Token::label_literal() const {
        return m_label_literal;
    }
}
