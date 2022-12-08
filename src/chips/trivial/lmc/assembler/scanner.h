#pragma once

#include "assembler/token_kind.h"
#include "token.h"
#include "usings.h"
#include <sstream>
#include <string>
#include <vector>

namespace emu::lmc {

class Scanner {
public:
    explicit Scanner(std::stringstream const& code);

    explicit Scanner(std::stringstream const& code, bool is_debugging);

    Token current_token();

    void skip(TokenKind next);

    Address current_address();

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

    static bool is_comment_line(std::string const& line);

    bool handle_single_character(std::string const& line);

    bool handle_number(std::string const& line);

    bool handle_keyword(std::string const& line);

    bool handle_inline_comment(std::string const& line);
};
}
