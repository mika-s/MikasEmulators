#ifndef MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include "crosscutting/typedefs.h"

namespace emu::util::string {

    std::string hexify(u8 val);

    std::string hexify(u16 val);

    std::string hexify_wo_0x(u8 val);

    std::string hexify_wo_0x(u16 val);

    std::string hexify_wo_0x(unsigned int val, int width);

    std::string find_short_executable_name(char *long_program_name);

    std::vector<std::string> split(const std::stringstream &ss, const std::string &delimiter);

    std::string prepend(std::string prefix, const char *txt);

    std::string append(std::string postfix, const char *txt);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
