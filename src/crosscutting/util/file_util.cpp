#include "file_util.h"
#include "crosscutting/exceptions/rom_file_not_found_exception.h"
#include "crosscutting/typedefs.h"
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep

namespace emu::util::file {

using emu::exceptions::RomFileNotFoundException;

auto convert_char_pointer_to_vector(const char* memory, const std::streampos size) -> std::vector<u8>
{
    std::vector<u8> program;
    program.reserve(size);

    for (int i = 0; i < size; ++i) {
        program.push_back(static_cast<u8>(memory[i]));
    }

    return program;
}

auto read_file_into_vector(std::string const& path) -> std::vector<u8>
{
    if (!std::filesystem::exists(path)) {
        throw RomFileNotFoundException(path);
    }

    char* memory;

    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

    std::vector<u8> program;

    if (file.is_open()) {
        std::streampos const size = file.tellg();
        memory = new char[static_cast<unsigned long>(size)];
        file.seekg(0, std::ios::beg);
        file.read(memory, size);
        file.close();

        program = convert_char_pointer_to_vector(memory, size);

        delete[] memory;
    } else {
        throw RomFileNotFoundException(path);
    }

    return program;
}

auto read_file(std::string const& path) -> std::stringstream
{
    std::string line;
    std::stringstream ss;
    std::ifstream file(path, std::ios::in);

    if (file.is_open()) {
        while (getline(file, line)) {
            ss << line << "\n";
        }
        file.close();
    } else {
        throw RomFileNotFoundException(path);
    }

    return ss;
}
}
