#pragma once

#include "crosscutting/typedefs.h"
#include <string>

namespace emu::debugger {
template<class A, std::size_t B>
class DisassembledLine;
}

namespace emu::debugger {

template<class A, std::size_t B>
class Breakpoint {
public:
    Breakpoint(A address, std::string line)
        : m_address(address)
        , m_line(std::move(line))
    {
    }

    explicit Breakpoint(DisassembledLine<A, B> line)
        : Breakpoint(line.address(), line.full_line())
    {
    }

    [[nodiscard]] std::string const& line() const
    {
        return m_line;
    }

private:
    [[maybe_unused]] A m_address;
    std::string m_line;
};
}
