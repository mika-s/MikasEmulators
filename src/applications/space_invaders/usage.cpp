#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace emu::applications::space_invaders {

using emu::util::string::create_padding;

static constexpr std::size_t padding_to_description = 14;

const std::vector<std::pair<std::string, std::string>> supported_flags = {
    { "-g", "ordinary, debugging. ordinary is default." },
    { "-d", "Dipswitches. See description below." }
};
const std::vector<std::pair<std::string, std::string>> supported_dipswitches = {
    { "n", "Number of lives: 3, 4, 5 or 6. 3 is default." },
    { "b", "Bonus life at: 1000 or 1500. 1500 is default." },
    { "c", "Coins needed: on or off. on is default." },
};
const std::vector<std::pair<std::string, std::string>> examples = {
    { "-g debugging -d b=1000", "Running with the debugging GUI and bonus life at 1000" },
    { "-d c=off -d n=6", "Coins not needed (this might not work) and 6 lives" }
};

void print_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run space_invaders [FLAGS]\n\n";
    std::cout << "Run Space Invaders on the 8080 CPU\n\n";

    std::cout << "Flags:\n";

    for (const auto&[flag, desc] : supported_flags) {
        std::string const padding = create_padding(flag.size(), padding_to_description);
        std::cout << "  " << flag << padding << desc << "\n";
    }

    std::cout << "\nDipswitches:\n";

    for (const auto&[dipswitch, desc] : supported_dipswitches) {
        std::string const padding = create_padding(dipswitch.size(), padding_to_description);
        std::cout << "  " << dipswitch << padding << desc << "\n";
    }

    std::cout << "\nExamples:\n";

    for (const auto&[example, desc] : examples) {
        std::cout << "  " << desc << ":\n";
        std::cout << "    "
                  << "./" << program_name << " run space_invaders " << example << "\n\n";
    }
}
}
