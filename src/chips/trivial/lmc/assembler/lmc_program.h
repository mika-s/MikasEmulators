#pragma once

#include "environment.h"
#include "lmc_line.h"
#include "usings.h"
#include <vector>

namespace emu::lmc {
class Scanner;
}

namespace emu::lmc {
class LmcProgram {
public:
    explicit LmcProgram(std::vector<LmcLine> lines);

    auto eval() const -> std::vector<Data>;

    static auto parse(Scanner& scanner, Environment& environment) -> LmcProgram;

private:
    std::vector<LmcLine> m_lines;
    Environment m_environment;
};
}
