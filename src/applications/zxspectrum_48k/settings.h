#pragma once

#include <string>

namespace emu::applications {
class Options;
}

namespace emu::applications::zxspectrum_48k {

class Settings {
public:
    std::string m_snapshot_file;
    bool m_is_only_printing_header;

    static Settings from_options(Options const& options);
};
}
