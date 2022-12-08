#pragma once

#include "lmc_integer.h"
#include "lmc_label_argument.h"
#include "usings.h"
#include <optional>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

class LmcOperand {
public:
    LmcOperand(std::optional<LmcInteger> integer_literal, std::optional<LmcLabelArgument> label_literal);

    Data eval();

    static LmcOperand parse(Scanner& scanner, Environment& environment);

private:
    std::optional<LmcInteger> m_integer_literal;
    std::optional<LmcLabelArgument> m_label_literal;
};
}
