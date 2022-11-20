#include "unrecognized_assembly_exception.h"
#include <sstream>

namespace emu::lmc {

    UnrecognizedAssemblyException::UnrecognizedAssemblyException(const std::string &assembly)
        : runtime_error("Unrecognized assembly") {
        make_message(assembly);
    }

    UnrecognizedAssemblyException::UnrecognizedAssemblyException(const std::string &assembly, const std::string &extra_message)
        : runtime_error("Unrecognized assembly (" + extra_message + ")") {
        make_message(assembly);
    }

    const char *UnrecognizedAssemblyException::what() const noexcept {
        return m_message.c_str();
    }

    void UnrecognizedAssemblyException::make_message(const std::string &assembly) {
        std::stringstream ss;
        ss << runtime_error::what() << ": code = " << assembly;

        m_message = ss.str();
    }
}
