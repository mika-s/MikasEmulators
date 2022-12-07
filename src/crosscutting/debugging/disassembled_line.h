#ifndef MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H
#define MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H

#include "crosscutting/typedefs.h"
#include <stdexcept>
#include <string>

namespace emu::debugger {
    template<class A, std::size_t B>
    class DisassembledLine {
    public:
        explicit DisassembledLine(const std::string &raw_line)
            : m_address(address_from_disassembly_line(raw_line)),
              m_full_line(raw_line) {
        }

        DisassembledLine(A address, std::string full_line)
            : m_address(address),
              m_full_line(std::move(full_line)) {
        }

        A address() {
            return m_address;
        }

        std::string full_line() {
            return m_full_line;
        }

    private:
        A m_address;
        std::string m_full_line;

        static A address_from_disassembly_line(std::string line) {
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

#endif //MIKA_EMULATORS_CROSSCUTTING_DEBUGGING_DISASSEMBLED_LINE_H
