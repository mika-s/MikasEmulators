#include "scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include "crosscutting/util/string_util.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace emu::lmc {

using emu::util::string::split;

Scanner::Scanner(std::stringstream const& code)
    : Scanner(code, false)
{
}

Scanner::Scanner(std::stringstream const& code, bool is_debugging)
    : m_real_line_no(0)
    , m_current_address(0)
    , m_is_debugging(is_debugging)
{
    m_code_lines = split(code, "\n");
}

Address Scanner::current_address()
{
    return m_current_address;
}

Token Scanner::current_token()
{
    while (tokens_current_line.empty()) {
        read_next_line();
    }

    return tokens_current_line[0];
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
    if (!tokens_current_line.empty()) {
        tokens_current_line.erase(tokens_current_line.begin());
    }
}

void Scanner::read_next_line()
{
    m_current_address++;
    tokens_current_line.clear();

    if (m_real_line_no >= m_code_lines.size()) {
        if (m_is_debugging) {
            std::cout << m_real_line_no + 1 << ": " << std::flush;
        }
        tokens_current_line.emplace_back(TokenKind::Eof);
    } else if (m_code_lines[m_real_line_no].empty() || is_comment_line(m_code_lines[m_real_line_no])) {
    } else {
        if (m_is_debugging) {
            std::cout << m_real_line_no + 1 << ": " << std::flush;
        }
        read_tokens(m_code_lines[m_real_line_no]);
        tokens_current_line.emplace_back(TokenKind::Newline);
    }

    if (m_is_debugging) {
        for (Token& token : tokens_current_line) {
            std::cout << token << " " << std::flush;
        }
        std::cout << "\n"
                  << std::flush;
    }

    ++m_real_line_no;
}

void Scanner::read_tokens(std::string const& line)
{
    m_current_pos = 0;

    while (m_current_pos < line.length()) {
        if (!(handle_single_character(line) || handle_number(line) || handle_keyword(line) || handle_inline_comment(line))) {
            throw std::invalid_argument(fmt::format("Unable to parse character(s) in position {} of arguments", m_current_pos));
        }
    }
}

bool Scanner::is_comment_line(std::string const& line)
{
    return line.starts_with("//");
}

bool Scanner::handle_single_character(std::string const& line)
{
    switch (line[m_current_pos]) {
    case '\n':
        tokens_current_line.emplace_back(TokenKind::Newline);
        ++m_current_pos;
        break;
    case '\t':
    case ' ':
        ++m_current_pos;
        break;
    default:
        return false;
    }

    return true;
}

bool Scanner::handle_number(std::string const& line)
{
    if (std::isdigit(line[m_current_pos])) {
        const std::size_t length = line.length();
        const std::size_t start = m_current_pos;

        while (m_current_pos + 1 < length && std::isdigit(line[m_current_pos + 1])) {
            ++m_current_pos;
        }

        ++m_current_pos;

        const std::string int_string = line.substr(start, m_current_pos - start);
        int const literal = std::atoi(int_string.c_str());

        tokens_current_line.emplace_back(TokenKind::Integer, literal);
        return true;
    } else {
        return false;
    }
}

bool Scanner::handle_keyword(std::string const& line)
{
    if (std::isalpha(line[m_current_pos])) {
        const std::size_t length = line.length();
        const std::size_t start = m_current_pos;

        while (m_current_pos + 1 < length && std::isalpha(line[m_current_pos + 1])) {
            ++m_current_pos;
        }

        ++m_current_pos;

        std::string keyword = line.substr(start, m_current_pos - start);
        std::string normalized_keyword = keyword;
        std::transform(normalized_keyword.begin(), normalized_keyword.end(), normalized_keyword.begin(), ::toupper);

        if (keyword_string_as_TokenKind.contains(normalized_keyword)) {
            tokens_current_line.emplace_back(keyword_string_as_TokenKind.at(normalized_keyword));
            return true;
        } else {
            tokens_current_line.emplace_back(TokenKind::Label, keyword);
            return true;
        }
    } else {
        return false;
    }
}

bool Scanner::handle_inline_comment(std::string const& line)
{
    if (line[m_current_pos] == '/' && m_current_pos != line.size() - 1 && line[m_current_pos + 1] == '/') {
        m_current_pos = line.length();
        return true;
    } else {
        return false;
    }
}
}
