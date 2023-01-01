#include "usage.h"
#include "crosscutting/util/string_util.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

namespace emu::applications::zxspectrum_48k {

using emu::util::string::create_padding;

static constexpr std::size_t padding_to_description = 20;

const std::vector<std::pair<std::string, std::string>> supported_flags = {
    { "-g", "ordinary, debugging. ordinary is default." },
    { "--print-header", "Print header of snapshot or tape file." }
};
const std::vector<std::pair<std::string, std::string>> examples = {
    { "-g debugging", "Running with the debugging GUI, starting with the system ROM only" },
    { "-g debugging mygame.z80", "Running with the debugging GUI, loading and starting mygame.z80 immediately" },
    { "--print-header mygame.z80", "Print the header of mygame.z80" }
};

void print_usage(std::string const& program_name)
{
    std::cout << "\nUsage: ./" << program_name << " run zx-spectrum-48k [FLAGS] [snapshot-or-tape-file]\n\n";
    std::cout << "Run SX Spectrum 48K on the Z80 CPU\n\n";

    std::cout << "Flags:\n";

    for (auto& flag_description : supported_flags) {
        std::string padding = create_padding(flag_description.first.size(), padding_to_description);
        std::cout << "  " << flag_description.first << padding << flag_description.second << "\n";
    }

    std::cout << "\nExamples:\n";

    for (auto& example_description : examples) {
        std::cout << "  " << example_description.second << ":\n";
        std::cout << "    "
                  << "./" << program_name << " run zx-spectrum-48k " << example_description.first << "\n\n";
    }
}
}
