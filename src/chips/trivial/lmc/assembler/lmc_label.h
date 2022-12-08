#pragma once

#include "usings.h"
#include <optional>
#include <string>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

class LmcLabel {
public:
    explicit LmcLabel(std::string literal, Environment& environment);

    Address eval();

    static std::optional<LmcLabel> parse(Scanner& scanner, Environment& environment);

private:
    std::string m_literal;
    Environment& m_environment;
};
}
