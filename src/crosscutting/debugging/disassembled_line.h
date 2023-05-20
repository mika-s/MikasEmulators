#pragma once

#include "crosscutting/typedefs.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace emu::debugger {
template<class A, std::size_t B>
class DisassembledLine {
public:
    explicit DisassembledLine(std::string const& raw_line)
        : m_address(address_from_disassembly_line(raw_line))
        , m_full_line(raw_line)
    {
    }

    DisassembledLine(A address, std::string full_line)
        : m_address(address)
        , m_full_line(std::move(full_line))
    {
    }

    [[nodiscard]] A address() const
    {
        return m_address;
    }

    [[nodiscard]] std::string const& full_line() const
    {
        return m_full_line;
    }

private:
    A m_address;
    std::string m_full_line;

    static A address_from_disassembly_line(std::string line)
    {
        const std::string delimiter = "\t";
        std::size_t pos;
        std::string token;
        std::vector<std::string> split;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            split.push_back(token);
            line.erase(0, pos + delimiter.length());
        }

        if (split.empty()) {
            throw std::runtime_error("Programming error: no elements in split disassembler line");
        }

        return A(std::stoi(split[0], nullptr, B));
    }
};
}
