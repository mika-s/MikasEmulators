#pragma once

#include "crosscutting/misc/uinteger.h"
#include "crosscutting/typedefs.h"
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace emu::util::string {

std::string hexify(u8 val);

std::string hexify(u16 val);

template<std::size_t M>
std::string hexify(emu::misc::UInteger<M> val)
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

    return ss.str();
}

std::string hexify_wo_0x(u8 val);

std::string hexify_wo_0x(i8 val);

std::string hexify_wo_0x(u16 val);

std::string hexify_wo_0x(unsigned int val, int width);

std::string find_short_executable_name(std::string name);

std::string create_padding(std::size_t length_so_far, std::size_t expected_length);

std::vector<std::string> split(std::stringstream const& ss, std::string const& delimiter);

std::string_view trim(std::string_view s);

std::string prepend(std::string prefix, char const* txt);

std::string append(std::string postfix, char const* txt);

bool is_alphanumeric(std::string const& str);

bool is_alpha(std::string const& str);
}
