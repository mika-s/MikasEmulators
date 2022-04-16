#include "string_util.h"

namespace emu::util::string {

    std::string hexify(std::uint8_t val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string returnVal = ss.str();

        return returnVal;
    }

    std::string hexify(std::uint16_t val) {
        std::stringstream ss;
        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << val;

        return ss.str();
    }

    std::string hexify_wo_0x(std::uint8_t val) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(val);
        std::string returnVal = ss.str();

        return returnVal;
    }

    std::string hexify_wo_0x(unsigned int val, int width) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(width) << std::hex << static_cast<int>(val);
        std::string returnVal = ss.str();

        return returnVal;
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

    std::vector<std::string> split(const std::stringstream &ss, const std::string& delimiter) {
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
}
