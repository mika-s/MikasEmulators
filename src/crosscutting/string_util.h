#ifndef MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H

#include <iomanip>
#include <string>
#include <sstream>

namespace emu::util::string {

    std::string hexify(std::uint8_t val);

    std::string hexify(std::uint16_t val);

    std::string hexify_wo_0x(std::uint8_t val);

    std::string hexify_wo_0x(unsigned int val, int width);

    std::string find_short_executable_name(char *long_program_name);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
