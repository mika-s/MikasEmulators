#pragma once

#include "token.h"
#include "usings.h"
#include <sstream>
#include <string>
#include <vector>

namespace emu::lmc {
enum class TokenKind;
}

namespace emu::lmc {

class Scanner {
public:
    explicit Scanner(std::stringstream const& code);

    explicit Scanner(std::stringstream const& code, bool is_debugging);

    auto current_token() -> Token;

    void skip(TokenKind next);

    [[nodiscard]] auto current_address() const -> Address;

private:
    unsigned int m_real_line_no;
    std::vector<std::string> m_code_lines;
    std::vector<Token> tokens_current_line;
    unsigned int m_current_pos = 0;
    Address m_current_address;
    bool m_is_debugging;

    void read_next_token();

    void read_next_line();

    void read_tokens(std::string const& line);

    static auto is_comment_line(std::string const& line) -> bool;

    auto handle_single_character(std::string const& line) -> bool;

    auto handle_number(std::string const& line) -> bool;

    auto handle_keyword(std::string const& line) -> bool;

    auto handle_inline_comment(std::string const& line) -> bool;
};
}
