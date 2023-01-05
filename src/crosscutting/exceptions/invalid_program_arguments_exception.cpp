#include "invalid_program_arguments_exception.h"
#include <sstream>
#include <utility>

namespace emu::exceptions {

InvalidProgramArgumentsException::InvalidProgramArgumentsException(
    std::string const& msg,
    std::function<void(std::string const& program_name)> usage_function)
    : std::runtime_error("Error in program arguments: ")
    , m_usage_function(std::move(usage_function))
{
    make_message(msg);
}

char const* InvalidProgramArgumentsException::what() const noexcept
{
    return m_message.c_str();
}

void InvalidProgramArgumentsException::make_message(std::string const& msg)
{
    std::stringstream ss;
    ss << runtime_error::what() << msg;

    m_message = ss.str();
}
std::function<void(const std::string& program_name)> const& InvalidProgramArgumentsException::usage_function() const
{
    return m_usage_function;
}
}
