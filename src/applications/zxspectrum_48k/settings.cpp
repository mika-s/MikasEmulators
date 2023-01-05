#include "settings.h"
#include "options.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace emu::applications::zxspectrum_48k {

Settings Settings::from_options(Options const& options)
{
    Settings settings {
        .m_snapshot_file = "",
        .m_is_only_printing_header = false
    };

    std::optional<std::string> path = options.path();

    if (path.has_value()) {
        settings.m_snapshot_file = path.value();
    }

    std::unordered_map<std::string, std::vector<std::string>> opts = options.options();

    if (opts.contains("print")) {
        settings.m_is_only_printing_header = true;
    }

    return settings;
}
}
