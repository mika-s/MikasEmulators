#pragma once

#include "crosscutting/typedefs.h"
#include <stdexcept>
#include <string>

namespace emu::exceptions {

class UnrecognizedOpcodeException : public std::runtime_error {
public:
    explicit UnrecognizedOpcodeException(u8 opcode);

    UnrecognizedOpcodeException(u8 opcode, std::string const& extra_message);

    explicit UnrecognizedOpcodeException(u16 opcode);

    UnrecognizedOpcodeException(u16 opcode, std::string const& extra_message);

    [[nodiscard]] char const* what() const noexcept override;

private:
    void make_message(u8 opcode);

    void make_message(u16 opcode);

    std::string m_message;
};
}
