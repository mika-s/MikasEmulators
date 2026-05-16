#include "string_util.h"
#include "crosscutting/typedefs.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <format>
#include <iomanip>
#include <stdexcept>
#include <tuple>

namespace emu::util::string {

auto hexify(const u8 val) -> std::string
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
    std::string return_val = ss.str();

    return return_val;
}

auto hexify(const u16 val) -> std::string
{
    std::stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

    return ss.str();
}

auto hexify_wo_0x(const u8 val) -> std::string
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
    std::string return_val = ss.str();

    return return_val;
}

auto hexify_wo_0x(const i8 val) -> std::string
{
    if (val >= 0) {
        return hexify_wo_0x(static_cast<u8>(val));
    }

    u8 const uval = static_cast<u8>(abs(val));
    std::stringstream ss;
    ss << "-" << hexify_wo_0x(uval);
    return ss.str();
}

auto hexify_wo_0x(const u16 val) -> std::string
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(val);
    std::string return_val = ss.str();

    return return_val;
}

auto hexify_wo_0x(const unsigned int val, const int width) -> std::string
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::hex << static_cast<int>(val);
    std::string return_val = ss.str();

    return return_val;
}

auto find_short_executable_name(std::string name) -> std::string
{
    const std::string delimiter = "/";

    std::size_t pos = 0;
    while ((pos = name.find(delimiter)) != std::string::npos) {
        std::ignore = name.substr(0, pos);
        name.erase(0, pos + delimiter.length());
    }

    return name;
}

auto create_padding(std::size_t length_so_far, std::size_t expected_length) -> std::string
{
    if (length_so_far > expected_length) {
        throw std::invalid_argument(
            std::format(
                "length_so_far cannot be larger than expected_length: {} > {}",
                length_so_far,
                expected_length));
    }

    std::stringstream ss;
    for (std::size_t i = 0; i < expected_length - length_so_far; ++i) {
        ss << " ";
    }

    return ss.str();
}

auto split(std::stringstream const& ss, std::string const& delimiter) -> std::vector<std::string>
{
    std::vector<std::string> split_string;
    std::string s = ss.str();
    std::size_t pos = 0;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        std::string const token = s.substr(0, pos);
        split_string.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    split_string.push_back(s);

    return split_string;
}

auto trim(std::string_view s) -> std::string_view
{
    s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
    s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));

    return s;
}

auto prepend(const std::string &prefix, char const* txt) -> std::string
{
    char* out = new char[strlen(txt) + prefix.size() + 1];

    for (std::size_t i = 0; i < prefix.size(); ++i) {
        out[i] = prefix[i];
    }

    for (std::size_t i = prefix.size(); i < strlen(txt) + prefix.size(); ++i) {
        out[i] = txt[i - prefix.size()];
    }

    out[strlen(txt) + prefix.size()] = '\0';

    return { out };
}

auto append(const std::string &postfix, char const* txt) -> std::string
{
    char* out = new char[strlen(txt) + postfix.size() + 1];

    for (std::size_t i = 0; i < strlen(txt); ++i) {
        out[i] = txt[i];
    }

    for (std::size_t i = strlen(txt); i < strlen(txt) + postfix.size(); ++i) {
        out[i] = postfix[i - strlen(txt)];
    }

    out[strlen(txt) + postfix.size()] = '\0';

    return { out };
}

auto is_alphanumeric(std::string const& str) -> bool
{
    return std::find_if(str.begin(), str.end(), [](char const& c) -> int { return isalnum(c); }) == str.end();
}

auto is_alpha(std::string const& str) -> bool
{
    return std::find_if(str.begin(), str.end(), [](char const& c) -> bool { return !isalpha(c); }) == str.end();
}
}
