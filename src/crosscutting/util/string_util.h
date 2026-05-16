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

auto hexify(u8 val) -> std::string;

auto hexify(u16 val) -> std::string;

template<std::size_t M>
auto hexify(emu::misc::UInteger<M> val) -> std::string
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

    return ss.str();
}

auto hexify_wo_0x(u8 val) -> std::string;

auto hexify_wo_0x(i8 val) -> std::string;

auto hexify_wo_0x(u16 val) -> std::string;

auto hexify_wo_0x(unsigned int val, int width) -> std::string;

auto find_short_executable_name(std::string name) -> std::string;

auto create_padding(std::size_t length_so_far, std::size_t expected_length) -> std::string;

auto split(std::stringstream const& ss, std::string const& delimiter) -> std::vector<std::string>;

auto trim(std::string_view s) -> std::string_view;

auto prepend(const std::string &prefix, char const* txt) -> std::string;

auto append(const std::string &postfix, char const* txt) -> std::string;

auto is_alphanumeric(std::string const& str) -> bool;

auto is_alpha(std::string const& str) -> bool;
}
