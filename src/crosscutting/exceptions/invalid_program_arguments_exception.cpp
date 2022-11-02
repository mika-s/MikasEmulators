#include "invalid_program_arguments_exception.h"
#include <sstream>

namespace emu::exceptions {

    InvalidProgramArgumentsException::InvalidProgramArgumentsException(
            const std::string &msg,
            std::function<void(const std::string &program_name)> usage_function
    )
        : runtime_error("Error in program arguments: "),
          m_usage_function(std::move(usage_function)) {
        make_message(msg);
    }

    const char *InvalidProgramArgumentsException::what() const noexcept {
        return m_message.c_str();
    }

    void InvalidProgramArgumentsException::make_message(const std::string &msg) {
        std::stringstream ss;
        ss << runtime_error::what() << msg;

        m_message = ss.str();
    }
    const std::function<void(const std::string &program_name)> &InvalidProgramArgumentsException::usage_function() const {
        return m_usage_function;
    }
}
