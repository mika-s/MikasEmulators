#include "scanner.h"
#include "command_line_arguments/token.h"
#include "command_line_arguments/token_kind.h"
#include <cassert>
#include <cctype>
#include <fmt/core.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace emu::applications::command_line_arguments {

Scanner::Scanner(std::vector<std::string> args)
    : m_args(std::move(args))
{
}

Token Scanner::current_token()
{
    while (tokens.empty()) {
        read_next_arg();
    }

    return tokens[0];
}

void Scanner::skip(TokenKind next)
{
    TokenKind current = current_token().kind();
    if (current != next) {
        throw std::invalid_argument(
            fmt::format(
                "Skipping wrong token. Was {}, but expecting {}.",
                TokenKind_as_string.at(current),
                TokenKind_as_string.at(next)));
    }
    read_next_token();
}

void Scanner::read_next_token()
{
    if (!tokens.empty()) {
        tokens.erase(tokens.begin());
    }
}

void Scanner::read_next_arg()
{
    if (m_arg_idx >= m_args.size()) {
        tokens.emplace_back(TokenKind::Eof);
    } else {
        read_token(m_args[m_arg_idx++]);
    }
}

void Scanner::read_token(std::string const& line)
{
    m_current_pos = 0;

    while (m_current_pos < line.length()) {
        if (!(handle_single_character(line) || handle_short_option(line) || handle_long_option(line) || handle_identifier(line))) {
            throw std::invalid_argument(fmt::format("Unable to parse character(s) in position {} of arguments", m_current_pos));
        }
    }
}

bool Scanner::handle_single_character(std::string const& line)
{
    switch (line[m_current_pos]) {
    case '=':
        tokens.emplace_back(TokenKind::Equals);
        ++m_current_pos;
        break;
    default:
        return false;
    }

    return true;
}

bool Scanner::handle_short_option(std::string const& line)
{
    if (line[m_current_pos] == '-') {
        bool const is_next_hyphen = m_current_pos < line.length() - 1 && line[m_current_pos + 1] == '-';
        if (is_next_hyphen) {
            return false;
        }

        if (m_current_pos + 1 < line.length() && std::isalnum(line[m_current_pos + 1])) {
            const std::string literal = std::string({ line[m_current_pos + 1] });

            assert(literal.length() != 0);

            tokens.emplace_back(TokenKind::ShortOption, literal);
            m_current_pos += 2;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Scanner::handle_long_option(std::string const& line)
{
    if (line[m_current_pos] == '-') {
        bool const is_next_hyphen = m_current_pos < line.length() - 1 && line[m_current_pos + 1] == '-';
        bool const is_next_next_alnum = m_current_pos < line.length() - 2 && std::isalnum(line[m_current_pos + 2]);
        if (!is_next_hyphen || !is_next_next_alnum) {
            return false;
        }

        ++m_current_pos;

        const std::size_t length = line.length();
        const std::size_t start = m_current_pos + 1;

        while (m_current_pos + 1 < length && std::isalnum(line[m_current_pos + 1])) {
            ++m_current_pos;
        }

        ++m_current_pos;

        const std::string literal = line.substr(start, m_current_pos - start);

        assert(literal.length() != 0);

        tokens.emplace_back(TokenKind::LongOption, literal);

        return true;
    } else {
        return false;
    }
}

bool Scanner::handle_identifier(std::string const& line)
{
    char current_char = line[m_current_pos];
    if (current_char != '-') {
        const std::size_t length = line.length();
        const std::size_t start = m_current_pos;

        while (m_current_pos + 1 < length && !std::isspace(line[m_current_pos + 1])) {
            ++m_current_pos;
        }

        ++m_current_pos;

        const std::string literal = line.substr(start, m_current_pos - start);
        tokens.emplace_back(TokenKind::Identifier, literal);
        return true;
    } else {
        return false;
    }
}
}
