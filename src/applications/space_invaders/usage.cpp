#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <iostream>
#include <vector>

namespace emu::applications::space_invaders {

    using emu::util::string::create_padding;

    static constexpr std::size_t padding_to_description = 14;

    const std::vector<std::pair<std::string, std::string>> supported_flags = {
            {"-g", "ordinary, debugging. ordinary is default."},
            {"-d", "Dipswitches. See description below."}
    };
    const std::vector<std::pair<std::string, std::string>> supported_dipswitches = {
            {"n", "Number of lives: 3, 4, 5 or 6. 3 is default."},
            {"b", "Bonus life at: 1000 or 1500. 1500 is default."},
            {"c", "Coins needed: on or off. on is default."},
    };
    const std::vector<std::pair<std::string, std::string>> examples = {
            {"-g debugging -d b=1000", "Running with the debugging GUI and bonus life at 1000"},
            {"-d c=off -d n=6", "Coins not needed (this might not work) and 6 lives"}
    };

    void print_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " run space_invaders [FLAGS]\n\n";
        std::cout << "Run Space Invaders on the 8080 CPU\n\n";

        std::cout << "Flags:\n";

        for (auto &flag_description: supported_flags) {
            std::string padding = create_padding(flag_description.first.size(), padding_to_description);
            std::cout << "  " << flag_description.first << padding << flag_description.second << "\n";
        }

        std::cout << "\nDipswitches:\n";

        for (auto &dipswitch_description: supported_dipswitches) {
            std::string padding = create_padding(dipswitch_description.first.size(), padding_to_description);
            std::cout << "  " << dipswitch_description.first << padding << dipswitch_description.second << "\n";
        }

        std::cout << "\nExamples:\n";

        for (auto &example_description: examples) {
            std::cout << "  " << example_description.second << ":\n";
            std::cout << "    "
                      << "./" << program_name << " run space_invaders " << example_description.first << "\n\n";
        }
    }
}
