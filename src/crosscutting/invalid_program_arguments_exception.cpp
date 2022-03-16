#include <sstream>
#include "invalid_program_arguments_exception.h"

namespace emu::util::exceptions {

    InvalidProgramArgumentsException::InvalidProgramArgumentsException(const std::string &msg)
            : runtime_error("Error in program arguments: ") {
        make_message(msg);
    }

    const char *InvalidProgramArgumentsException::what() const noexcept {
        return message.c_str();
    }

    void InvalidProgramArgumentsException::make_message(const std::string &msg) {
        std::stringstream ss;
        ss << runtime_error::what() << msg;

        message = ss.str();
    }
}
