#include "file_util.h"
#include "crosscutting/exceptions/rom_file_not_found_exception.h"
#include "crosscutting/typedefs.h"
#include <filesystem>
#include <fstream> // IWYU pragma: keep
#include <sstream> // IWYU pragma: keep

namespace emu::util::file {

using emu::exceptions::RomFileNotFoundException;

std::vector<u8> convert_char_pointer_to_vector(char* memory, std::streampos size)
{
    std::vector<u8> program;
    program.reserve(static_cast<unsigned long>(size));

    for (int i = 0; i < size; ++i) {
        program.push_back(static_cast<u8>(memory[i]));
    }

    return program;
}

std::vector<u8> read_file_into_vector(std::string const& path)
{
    if (!std::filesystem::exists(path)) {
        throw RomFileNotFoundException(path);
    }

    std::streampos size;
    char* memory;

    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

    std::vector<u8> program;

    if (file.is_open()) {
        size = file.tellg();
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

std::stringstream read_file(std::string const& path)
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
