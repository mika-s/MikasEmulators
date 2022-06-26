#include <fstream>
#include <iostream>
#include "file_util.h"

namespace emu::util::file {

    std::vector<u8> convert_char_pointer_to_vector(char *memory, std::streampos size) {
        std::vector<u8> program;
        program.reserve(size);

        for (int i = 0; i < size; ++i) {
            program.push_back(memory[i]);
        }

        return program;
    }

    std::vector<u8> read_file_into_vector(const std::string &path) {
        std::streampos size;
        char *memory;

        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

        std::vector<u8> program;

        if (file.is_open()) {
            size = file.tellg();
            memory = new char[size];
            file.seekg(0, std::ios::beg);
            file.read(memory, size);
            file.close();

            program = convert_char_pointer_to_vector(memory, size);

            delete[] memory;
        } else {
            std::cout << "Unable to open file: " << path << "\n";
        }

        return program;
    }
}
