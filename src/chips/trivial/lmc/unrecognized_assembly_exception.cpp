#include "unrecognized_assembly_exception.h"
#include <sstream>

namespace emu::lmc {

UnrecognizedAssemblyException::UnrecognizedAssemblyException(std::string const& assembly)
    : runtime_error("Unrecognized assembly")
{
    make_message(assembly);
}

UnrecognizedAssemblyException::UnrecognizedAssemblyException(std::string const& assembly, std::string const& extra_message)
    : runtime_error("Unrecognized assembly (" + extra_message + ")")
{
    make_message(assembly);
}

char const* UnrecognizedAssemblyException::what() const noexcept
{
    return m_message.c_str();
}

void UnrecognizedAssemblyException::make_message(std::string const& assembly)
{
    std::stringstream ss;
    ss << runtime_error::what() << ": code = " << assembly;

    m_message = ss.str();
}
}
