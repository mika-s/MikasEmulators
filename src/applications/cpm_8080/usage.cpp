#include "usage.h"
#include <iostream>

namespace emu::applications::cpm::i8080 {

void print_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run [CP/M application]\n\n";
    std::cout << "Run a CP/M application on the 8080 CPU\n\n";
}
}
