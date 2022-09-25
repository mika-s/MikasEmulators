#include <cstring>
#include "string_util.h"

namespace emu::util::string {

    std::string hexify(u8 val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string returnVal = ss.str();

        return returnVal;
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
            u8 uval = abs(val);
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

    std::string find_short_executable_name(char *long_program_name) {
        std::string name(long_program_name);
        const std::string delimiter = "/";

        size_t pos;
        std::string token;
        while ((pos = name.find(delimiter)) != std::string::npos) {
            name.substr(0, pos);
            name.erase(0, pos + delimiter.length());
        }

        return name;
    }

    std::vector<std::string> split(const std::stringstream &ss, const std::string &delimiter) {
        std::vector<std::string> split_string;
        std::string s = ss.str();
        std::string token;
        size_t pos;

        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            split_string.push_back(token);
            s.erase(0, pos + delimiter.length());
        }

        return split_string;
    }

    std::string prepend(std::string prefix, const char *txt) {
        char *out = new char[strlen(txt) + prefix.size() + 1];

        for (size_t i = 0; i < prefix.size(); ++i) {
            out[i] = prefix[i];
        }

        for (size_t i = prefix.size(); i < strlen(txt) + prefix.size(); ++i) {
            out[i] = txt[i - prefix.size()];
        }

        out[strlen(txt) + prefix.size()] = '\0';

        return {out};
    }

    std::string append(std::string postfix, const char *txt) {
        char *out = new char[strlen(txt) + postfix.size() + 1];

        for (size_t i = 0; i < strlen(txt); ++i) {
            out[i] = txt[i];
        }

        for (size_t i = strlen(txt); i < strlen(txt) + postfix.size(); ++i) {
            out[i] = postfix[i - strlen(txt)];
        }

        out[strlen(txt) + postfix.size()] = '\0';

        return {out};
    }
}
