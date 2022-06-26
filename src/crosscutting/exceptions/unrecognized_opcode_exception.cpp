#include <sstream>
#include "unrecognized_opcode_exception.h"
#include "crosscutting/util/string_util.h"

namespace emu::exceptions {

    using namespace emu::util::string;

    UnrecognizedOpcodeException::UnrecognizedOpcodeException(u8 opcode)
            : runtime_error("Unrecognized opcode") {
        make_message(opcode);
    }

    const char *UnrecognizedOpcodeException::what() const noexcept {
        return m_message.c_str();
    }

    void UnrecognizedOpcodeException::make_message(u8 opcode) {
        std::stringstream ss;
        ss << runtime_error::what() << ": opcode = " << hexify(opcode);

        m_message = ss.str();
    }
}
