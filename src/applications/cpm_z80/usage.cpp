#include "usage.h"
#include <iostream>

namespace emu::applications::cpm::z80 {

    void print_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " run [CP/M application]\n\n";
        std::cout << "Run a CP/M application on the Z80 CPU\n\n";
    }
}
