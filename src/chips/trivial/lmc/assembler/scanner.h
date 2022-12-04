#ifndef MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_SCANNER_H
#define MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_SCANNER_H

#include "assembler/token_kind.h"
#include "token.h"
#include "usings.h"
#include <sstream>
#include <string>
#include <vector>

namespace emu::lmc {

    class Scanner {
    public:
        explicit Scanner(const std::stringstream &code);

        explicit Scanner(const std::stringstream &code, bool is_debugging);

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

        void read_tokens(const std::string &line);

        static bool is_comment_line(const std::string &line);

        bool handle_single_character(const std::string &line);

        bool handle_number(const std::string &line);

        bool handle_keyword(const std::string &line);

        bool handle_inline_comment(const std::string &line);
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_ASSEMBLER_SCANNER_H
