#ifndef MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H

#include <vector>
#include <string>
#include "crosscutting/typedefs.h"

namespace emu::util::file {
    std::vector<u8> read_file_into_vector(const std::string &path);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_FILE_UTIL_H
