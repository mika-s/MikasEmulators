#include "rom_file_not_found_exception.h"
#include <sstream>

namespace emu::exceptions {

RomFileNotFoundException::RomFileNotFoundException(std::string const& message)
    : runtime_error("ROM file not found or unable to be opened: ")
{
    make_message(message);
}

auto RomFileNotFoundException::what() const noexcept -> char const*
{
    return m_message.c_str();
}

void RomFileNotFoundException::make_message(std::string const& message)
{
    std::stringstream ss; // NOLINT(*-identifier-length)
    ss << runtime_error::what() << message;

    m_message = ss.str();
}
}
