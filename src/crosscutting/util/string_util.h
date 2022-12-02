#ifndef MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
#define MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H

#include "crosscutting/typedefs.h"
#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace emu::util::string {

    std::string hexify(u8 val);

    std::string hexify(u16 val);

    std::string hexify_wo_0x(u8 val);

    std::string hexify_wo_0x(i8 val);

    std::string hexify_wo_0x(u16 val);

    std::string hexify_wo_0x(unsigned int val, int width);

    std::string find_short_executable_name(std::string name);

    std::string create_padding(std::size_t length_so_far, std::size_t expected_length);

    std::vector<std::string> split(const std::stringstream &ss, const std::string &delimiter);

    std::string_view trim(std::string_view s);

    std::string prepend(std::string prefix, const char *txt);

    std::string append(std::string postfix, const char *txt);

    bool is_alphanumeric(const std::string &str);

    bool is_alpha(const std::string &str);
}

#endif //MIKA_EMULATORS_CROSSCUTTING_STRING_UTIL_H
