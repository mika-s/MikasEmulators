#include "lmc_program.h"
#include "assembler/lmc_line.h"
#include "assembler/scanner.h"
#include "assembler/token.h"
#include "assembler/token_kind.h"
#include <utility>

namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

LmcProgram::LmcProgram(std::vector<LmcLine> lines)
    : m_lines(std::move(lines))
{
}

std::vector<Data> LmcProgram::eval()
{
    std::vector<Data> data;

    for (LmcLine& line : m_lines) {
        data.push_back(line.eval());
    }

    return data;
}

LmcProgram LmcProgram::parse(Scanner& scanner, Environment& environment)
{
    std::vector<LmcLine> lines;

    while (scanner.current_token().kind() != TokenKind::Eof) {
        lines.emplace_back(LmcLine::parse(scanner, environment));
    }

    return LmcProgram(std::move(lines));
}
}
