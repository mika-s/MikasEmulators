#include "disassembled_line.h"
#include <stdexcept>
#include <vector>

namespace emu::debugger {

    DisassembledLine::DisassembledLine(const std::string& raw_line)
            : m_address(address_from_disassembly_line(raw_line)),
              m_full_line(raw_line) {
    }

    DisassembledLine::DisassembledLine(u16 address, std::string full_line)
            : m_address(address),
              m_full_line(std::move(full_line)) {
    }

    u16 DisassembledLine::address() {
        return m_address;
    }

    std::string DisassembledLine::full_line() {
        return m_full_line;
    }


    u16 DisassembledLine::address_from_disassembly_line(std::string line) {
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

        return static_cast<u16>(std::stoi(split[0], nullptr, address_base));
    }
}
