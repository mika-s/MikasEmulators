#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace emu::applications::pacman {

    using emu::util::string::create_padding;

    static constexpr std::size_t padding_to_description = 14;

    const std::vector<std::pair<std::string, std::string>> supported_flags = {
            {"-g", "ordinary, debugging. ordinary is default."},
            {"-d", "Dipswitches. See description below."}
    };
    const std::vector<std::pair<std::string, std::string>> supported_dipswitches = {
            {"n", "Number of lives: 1, 2, 3 or 5. 3 is default."},
            {"b", "Bonus life at: 10000, 15000, 20000 or none. 15000 is default."},
            {"c", "Coins per game: 1C1G (one coin per game), 1C2G (one coin per two games), 2C1G (two coins per game) or free. 1C1G is default."},
            {"d", "Difficulty: normal or hard. normal is default."},
            {"g", "Ghost names: normal or alternate. normal is default."},
            {"t", "Board test: on or off. off is default."},
            {"m", "Cabinet mode: table or upright. upright is default"}
    };
    const std::vector<std::pair<std::string, std::string>> examples = {
            {"-g debugging -d b=20000 -d g=alternate", "Running with the debugging GUI, bonus life at 20000 and alternate ghost names"},
            {"-d m=table -d d=hard -c=free", "Running with table cabinet mode, difficulty hard and playing for free"}};

    void print_usage(const std::string &program_name) {
        std::cout << "\nUsage: ./" << program_name << " run pacman [FLAGS]\n\n";
        std::cout << "Run Pacman on the Z80 CPU\n\n";

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
                      << "./" << program_name << " run pacman " << example_description.first << "\n\n";
        }
    }
}
