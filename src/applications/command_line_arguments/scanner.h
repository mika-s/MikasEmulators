#pragma once

#include "token.h"
#include "token_kind.h"
#include <cstddef>
#include <string>
#include <vector>

namespace emu::applications::command_line_arguments {

class Scanner {
public:
    explicit Scanner(std::vector<std::string> args);

    Token current_token();

    void skip(TokenKind next);

    std::vector<std::string> tokens_as_strings();

private:
    static constexpr char s_start_of_flag = '-';

    std::vector<std::string> m_args;
    std::vector<Token> m_tokens;
    std::vector<Token> m_all_scanned_tokens;
    std::size_t m_current_pos = 0;
    std::size_t m_arg_idx = 0;

    void read_next_token();

    void read_next_arg();

    void read_token(std::string const& line);

    bool handle_single_character(std::string const& line);

    bool handle_short_option(std::string const& line);

    bool handle_long_option(std::string const& line);

    bool handle_identifier(std::string const& line);
};
}
