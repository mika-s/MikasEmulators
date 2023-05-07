#include "settings.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "options.h"
#include "usage.h"
#include <fmt/core.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications::game_boy {

using emu::exceptions::InvalidProgramArgumentsException;

Settings Settings::from_options(Options const& options)
{
    using namespace applications::game_boy;

    for (auto const& opt : options.options()) {
        if (!s_recognized_options.contains(opt.first)) {
            throw InvalidProgramArgumentsException(fmt::format("Unknown flag: {}", opt.first), print_usage);
        }
    }

    Settings settings {};

    std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

    return settings;
}
}
