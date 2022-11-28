#include "assembler.h"
#include "assembler/environment.h"
#include "chips/trivial/lmc/usings.h"
#include "crosscutting/misc/uinteger.h"
#include "lmc_program.h"
#include "scanner.h"
#include <iostream>

namespace emu::lmc {

    std::vector<Data> Assembler::assemble(const std::stringstream &code) {
        Scanner scanner(code);
        Environment environment;
        LmcProgram program = LmcProgram::parse(scanner, environment);
        std::vector<Data> assembled_code = program.eval();

        for (Data value: assembled_code) {
            std::cout << value << " ";
        }
        std::cout << "\n";

        return assembled_code;
    }
}
