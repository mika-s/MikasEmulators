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

auto InvalidProgramArgumentsException::what() const noexcept -> char const*
{
    return m_message.c_str();
}

void InvalidProgramArgumentsException::make_message(std::string const& msg)
{
    std::stringstream ss; // NOLINT(*-identifier-length)
    ss << runtime_error::what() << msg;

    m_message = ss.str();
}

auto InvalidProgramArgumentsException::usage_function() const -> std::function<void(std::string const& program_name)> const&
{
    return m_usage_function;
}
}
