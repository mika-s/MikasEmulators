#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace emu::applications::lmc {

using emu::util::string::create_padding;

static constexpr std::size_t padding_to_description = 14;

const std::vector<std::pair<std::string, std::string>> supported_flags = {
    { "-g", "ordinary, debugging. ordinary is default." }
};
const std::vector<std::pair<std::string, std::string>> examples = {
    { "-g debugging add_numbers.lmc", "Running add_numbers.lmc with the debugging GUI" },
    { "-g ordinary test.lmc", "Running test.lmc in the terminal" }
};

void print_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run lmc_application [FLAGS] <file>\n\n";
    std::cout << "Run a LMC program\n\n";

    std::cout << "Flags:\n";

    for (auto& flag_description : supported_flags) {
        std::string padding = create_padding(flag_description.first.size(), padding_to_description);
        std::cout << "  " << flag_description.first << padding << flag_description.second << "\n";
    }

    std::cout << "\nExamples:\n";

    for (auto& example_description : examples) {
        std::cout << "  " << example_description.second << ":\n";
        std::cout << "    "
                  << "./" << program_name << " run lmc_application " << example_description.first << "\n\n";
    }
}
}
