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

private:
    std::vector<std::string> m_args;
    std::vector<Token> tokens;
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
