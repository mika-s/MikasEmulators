#include "unsupported_exception.h"
#include <sstream>

namespace emu::exceptions {

UnsupportedException::UnsupportedException(std::string const& message)
    : std::invalid_argument("Unsupported: ")
{
    make_message(message);
}

auto UnsupportedException::what() const noexcept -> char const*
{
    return m_message.c_str();
}

void UnsupportedException::make_message(std::string const& message)
{
    std::stringstream ss; // NOLINT(*-identifier-length)
    ss << invalid_argument::what() << message;

    m_message = ss.str();
}
}
