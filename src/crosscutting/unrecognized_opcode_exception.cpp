#include <sstream>
#include "unrecognized_opcode_exception.h"
#include "crosscutting/string_util.h"

using namespace emu::util::string;

namespace emu::util::exceptions {

    UnrecognizedOpcodeException::UnrecognizedOpcodeException(std::uint8_t opcode)
            : runtime_error("Unrecognized opcode") {
        make_message(opcode);
    }

    const char *UnrecognizedOpcodeException::what() const noexcept {
        return m_message.c_str();
    }

    void UnrecognizedOpcodeException::make_message(std::uint8_t opcode) {
        std::stringstream ss;
        ss << runtime_error::what() << ": opcode = " << hexify(opcode);

        m_message = ss.str();
    }
}
