#pragma once

#include "usings.h"

namespace emu::lmc {
class Scanner;
}

namespace emu::lmc {

class LmcInteger {
public:
    explicit LmcInteger(int literal);

    Data eval();

    static LmcInteger parse(Scanner& scanner);

private:
    [[maybe_unused]] int m_literal;
};
}
