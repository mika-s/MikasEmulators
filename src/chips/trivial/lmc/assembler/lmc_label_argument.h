#pragma once

#include "usings.h"
#include <string>

namespace emu::lmc {
class Scanner;
}
namespace emu::lmc {
class Environment;
}

namespace emu::lmc {

class LmcLabelArgument {
public:
    explicit LmcLabelArgument(std::string literal, Environment& environment);

    Address eval();

    static LmcLabelArgument parse(Scanner& scanner, Environment& environment);

private:
    std::string m_literal;
    Environment& m_environment;
};
}
