#include "settings.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "options.h"
#include "usage.h"
#include <fmt/core.h>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications::zxspectrum_48k {

using emu::exceptions::InvalidProgramArgumentsException;

Settings Settings::from_options(Options const& options)
{
    for (auto const& opt : options.options()) {
        if (!s_recognized_options.contains(opt.first)) {
            throw InvalidProgramArgumentsException(fmt::format("Unknown flag: {}", opt.first), print_usage);
        }
    }

    Settings settings {
        .m_snapshot_file = "",
        .m_is_only_printing_header = false
    };

    const std::optional<std::string> path = options.path();

    if (path.has_value()) {
        settings.m_snapshot_file = path.value();
    }

    const std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

    if (opts.contains("print-header")) {
        settings.m_is_only_printing_header = true;
    }

    return settings;
}
}
