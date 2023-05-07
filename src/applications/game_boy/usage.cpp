#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace emu::applications::game_boy {

using emu::util::string::create_padding;

static constexpr std::size_t padding_to_description = 14;

const std::vector<std::pair<std::string, std::string>> supported_flags = {
    { "-g", "ordinary, debugging. ordinary is default." },
};
const std::vector<std::pair<std::string, std::string>> examples = {
    { "-g debugging", "Running with the debugging GUI" },
};

void print_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run game_boy [FLAGS]\n\n";
    std::cout << "Run Game Boy on the LR35902 CPU\n\n";

    std::cout << "Flags:\n";

    for (auto& flag_description : supported_flags) {
        std::string padding = create_padding(flag_description.first.size(), padding_to_description);
        std::cout << "  " << flag_description.first << padding << flag_description.second << "\n";
    }

    std::cout << "\nExamples:\n";

    for (auto& example_description : examples) {
        std::cout << "  " << example_description.second << ":\n";
        std::cout << "    "
                  << "./" << program_name << " run game_boy " << example_description.first << "\n\n";
    }
}
}
