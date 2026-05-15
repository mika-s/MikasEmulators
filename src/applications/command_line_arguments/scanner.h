#pragma once

#include "token.h"
#include <cstddef>
#include <string>
#include <vector>

namespace emu::applications::command_line_arguments {
enum class TokenKind;
}

namespace emu::applications::command_line_arguments {

class Scanner {
public:
    explicit Scanner(std::vector<std::string> args);

    auto current_token() -> Token;

    void skip(TokenKind next);

    auto tokens_as_strings() -> std::vector<std::string>;

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

    auto handle_single_character(std::string const& line) -> bool;

    auto handle_short_option(std::string const& line) -> bool;

    auto handle_long_option(std::string const& line) -> bool;

    auto handle_identifier(std::string const& line) -> bool;
};
}
