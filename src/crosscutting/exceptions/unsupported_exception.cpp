#include "unsupported_exception.h"
#include <sstream>

namespace emu::exceptions {

UnsupportedException::UnsupportedException(std::string const& msg)
    : std::invalid_argument("Unsupported: ")
{
    make_message(msg);
}

char const* UnsupportedException::what() const noexcept
{
    return m_message.c_str();
}

void UnsupportedException::make_message(std::string const& msg)
{
    std::stringstream ss;
    ss << invalid_argument::what() << msg;

    m_message = ss.str();
}
}
