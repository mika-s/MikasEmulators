#pragma once

#include "chips/trivial/lmc/usings.h"
#include <sstream>
#include <vector>

namespace emu::lmc {

/**
 * A simple assembler that parses the file line by line.
 * It does not use a fancy parsing algorithm, just split etc.
 */
class Assembler {
public:
    static std::vector<Data> assemble(std::stringstream const& code);

private:
};
}
