#ifndef MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H

#include <vector>
#include <string>

namespace emu::util::file {
    std::vector<std::uint8_t> read_file_into_vector(const std::string &path);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H
