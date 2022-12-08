#include "assembler.h"
#include "assembler/environment.h"
#include "chips/trivial/lmc/usings.h"
#include "lmc_program.h"
#include "scanner.h"

namespace emu::lmc {

std::vector<Data> Assembler::assemble(std::stringstream const& code)
{
    Scanner scanner(code);
    Environment environment;
    LmcProgram program = LmcProgram::parse(scanner, environment);
    std::vector<Data> assembled_code = program.eval();

    return assembled_code;
}
}
