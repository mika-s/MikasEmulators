#include <sstream>
#include "string_util.h"
#include "unrecognized_opcode_exception.h"

using namespace emu::util::string;

namespace emu::util::exceptions {

    UnrecognizedOpcodeException::UnrecognizedOpcodeException(std::uint8_t opcode)
            : runtime_error("Unrecognized opcode") {
        make_message(opcode);
    }

    const char *UnrecognizedOpcodeException::what() const noexcept {
        return message.c_str();
    }

    void UnrecognizedOpcodeException::make_message(std::uint8_t opcode) {
        std::stringstream ss;
        ss << runtime_error::what() << ": opcode = " << hexify(opcode);

        message = ss.str();
    }
}
