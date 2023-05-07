#pragma once

#include <string>
#include <unordered_set>

namespace emu::applications {
class Options;
}

namespace emu::applications::game_boy {

class Settings {
public:

    static Settings from_options(Options const& options);

private:
    static const inline std::string s_help_short = "h";
    static const inline std::string s_help_long = "help";
    static const inline std::string s_debug_scanner_long = "debug-scanner";

    static const inline std::string s_gui_short = "g";

    static const inline std::unordered_set<std::string> s_recognized_options = {
        s_help_short, s_help_long, s_debug_scanner_long, s_gui_short
    };
};
}
