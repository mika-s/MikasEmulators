#include "assembler.h"
#include "crosscutting/util/string_util.h"
#include "unrecognized_assembly_exception.h"
#include <string>
#include <string_view>

namespace emu::lmc {

    using emu::util::string::split;
    using emu::util::string::trim;

    std::vector<u16> Assembler::assemble(const std::stringstream &code) {
        std::vector<std::string> code_lines = split(code, "\n");
        std::vector<u16> assembled_code;

        for (const std::string &raw_line: code_lines) {
            std::string trimmed_line = std::string(trim(raw_line));
            std::stringstream trimmed_line_ss(trimmed_line);

            if (trimmed_line.empty()) {
                continue;
            } else if (trimmed_line.size() < 3) {
                throw UnrecognizedAssemblyException(trimmed_line);
            }

            if (trimmed_line.starts_with("ADD")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(100 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("SUB")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(200 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("STA")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(300 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("LDA")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(500 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("BRA")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(600 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("BRZ")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(700 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("BRP")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 2) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                const int address = std::stoi(add_line[1]);
                if (address < 0 || 99 < address) {
                    throw UnrecognizedAssemblyException(trimmed_line, "address too large");
                }

                assembled_code.push_back(800 + static_cast<u8>(address));
            } else if (trimmed_line.starts_with("INP")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 1) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                if (trimmed_line != "INP") {
                    throw UnrecognizedAssemblyException(trimmed_line);
                }

                assembled_code.push_back(901);
            } else if (trimmed_line.starts_with("OUT")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 1) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                if (trimmed_line != "OUT") {
                    throw UnrecognizedAssemblyException(trimmed_line);
                }

                assembled_code.push_back(902);
            } else if (trimmed_line.starts_with("HLT")) {
                std::vector<std::string> add_line = split(trimmed_line_ss, " ");
                if (add_line.size() != 1) {
                    throw UnrecognizedAssemblyException(trimmed_line, "too many arguments");
                }

                if (trimmed_line != "HLT") {
                    throw UnrecognizedAssemblyException(trimmed_line);
                }

                assembled_code.push_back(0);
            }
        }

        return assembled_code;
    }
}
