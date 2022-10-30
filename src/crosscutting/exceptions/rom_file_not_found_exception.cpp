#include <sstream>
#include "rom_file_not_found_exception.h"

namespace emu::exceptions {

    RomFileNotFoundException::RomFileNotFoundException(const std::string &file)
            : runtime_error("ROM file not found or unable to be opened: ") {
        make_message(file);
    }

    const char *RomFileNotFoundException::what() const noexcept {
        return m_message.c_str();
    }

    void RomFileNotFoundException::make_message(const std::string &file) {
        std::stringstream ss;
        ss << runtime_error::what() << file;

        m_message = ss.str();
    }
}
