#include "rom_file_not_found_exception.h"
#include <sstream>

namespace emu::exceptions {

RomFileNotFoundException::RomFileNotFoundException(std::string const& file)
    : runtime_error("ROM file not found or unable to be opened: ")
{
    make_message(file);
}

char const* RomFileNotFoundException::what() const noexcept
{
    return m_message.c_str();
}

void RomFileNotFoundException::make_message(std::string const& file)
{
    std::stringstream ss;
    ss << runtime_error::what() << file;

    m_message = ss.str();
}
}
