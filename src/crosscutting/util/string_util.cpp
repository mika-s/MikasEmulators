#include "string_util.h"
#include "crosscutting/typedefs.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fmt/core.h>
#include <iomanip>
#include <stdexcept>

namespace emu::util::string {

    std::string hexify(u8 val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string return_val = ss.str();

        return return_val;
    }

    std::string hexify(u16 val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

        return ss.str();
    }

    std::string hexify_wo_0x(u8 val) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string return_val = ss.str();

        return return_val;
    }

    std::string hexify_wo_0x(i8 val) {
        if (val >= 0) {
            return hexify_wo_0x(static_cast<u8>(val));
        } else {
            u8 uval = static_cast<u8>(abs(val));
            std::stringstream ss;
            ss << "-" << hexify_wo_0x(uval);
            return ss.str();
        }
    }

    std::string hexify_wo_0x(u16 val) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << std::hex << static_cast<int>(val);
        std::string return_val = ss.str();

        return return_val;
    }

    std::string hexify_wo_0x(unsigned int val, int width) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(width) << std::hex << static_cast<int>(val);
        std::string return_val = ss.str();

        return return_val;
    }

    std::string find_short_executable_name(std::string name) {
        const std::string delimiter = "/";

        std::size_t pos;
        std::string token;
        while ((pos = name.find(delimiter)) != std::string::npos) {
            name.substr(0, pos);
            name.erase(0, pos + delimiter.length());
        }

        return name;
    }

    std::string create_padding(std::size_t length_so_far, std::size_t expected_length) {
        if (length_so_far > expected_length) {
            throw std::invalid_argument(
                    fmt::format(
                            "length_so_far cannot be larger than expected_length: {} > {}",
                            length_so_far,
                            expected_length
                    )
            );
        }

        std::stringstream ss;
        for (std::size_t i = 0; i < expected_length - length_so_far; ++i) {
            ss << " ";
        }

        return ss.str();
    }

    std::vector<std::string> split(const std::stringstream &ss, const std::string &delimiter) {
        std::vector<std::string> split_string;
        std::string s = ss.str();
        std::string token;
        std::size_t pos;

        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            split_string.push_back(token);
            s.erase(0, pos + delimiter.length());
        }

        split_string.push_back(s);

        return split_string;
    }

    std::string_view trim(std::string_view s) {
        s.remove_prefix(std::min(s.find_first_not_of(" \t\r\v\n"), s.size()));
        s.remove_suffix(std::min(s.size() - s.find_last_not_of(" \t\r\v\n") - 1, s.size()));

        return s;
    }

    std::string prepend(std::string prefix, const char *txt) {
        char *out = new char[strlen(txt) + prefix.size() + 1];

        for (std::size_t i = 0; i < prefix.size(); ++i) {
            out[i] = prefix[i];
        }

        for (std::size_t i = prefix.size(); i < strlen(txt) + prefix.size(); ++i) {
            out[i] = txt[i - prefix.size()];
        }

        out[strlen(txt) + prefix.size()] = '\0';

        return {out};
    }

    std::string append(std::string postfix, const char *txt) {
        char *out = new char[strlen(txt) + postfix.size() + 1];

        for (std::size_t i = 0; i < strlen(txt); ++i) {
            out[i] = txt[i];
        }

        for (std::size_t i = strlen(txt); i < strlen(txt) + postfix.size(); ++i) {
            out[i] = postfix[i - strlen(txt)];
        }

        out[strlen(txt) + postfix.size()] = '\0';

        return {out};
    }

    bool is_alphanumeric(const std::string &str) {
        return std::find_if(str.begin(), str.end(), [](const char &c) { return isalnum(c); }) == str.end();
    }

    bool is_alpha(const std::string &str) {
        return std::find_if(str.begin(), str.end(), [](const char &c) { return !isalpha(c); }) == str.end();
    }
}
