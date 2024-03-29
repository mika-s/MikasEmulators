#include "unrecognized_opcode_exception.h"
#include "crosscutting/typedefs.h"
#include "crosscutting/util/string_util.h"
#include <sstream>

namespace emu::exceptions {

using namespace emu::util::string;

UnrecognizedOpcodeException::UnrecognizedOpcodeException(u8 opcode)
    : runtime_error("Unrecognized opcode")
{
    make_message(opcode);
}

UnrecognizedOpcodeException::UnrecognizedOpcodeException(u8 opcode, std::string const& extra_message)
    : runtime_error("Unrecognized opcode (" + extra_message + ")")
{
    make_message(opcode);
}

UnrecognizedOpcodeException::UnrecognizedOpcodeException(u16 opcode)
    : runtime_error("Unrecognized opcode")
{
    make_message(opcode);
}

UnrecognizedOpcodeException::UnrecognizedOpcodeException(u16 opcode, std::string const& extra_message)
    : runtime_error("Unrecognized opcode (" + extra_message + ")")
{
    make_message(opcode);
}

char const* UnrecognizedOpcodeException::what() const noexcept
{
    return m_message.c_str();
}

void UnrecognizedOpcodeException::make_message(u8 opcode)
{
    std::stringstream ss;
    ss << runtime_error::what() << ": opcode = " << hexify(opcode);

    m_message = ss.str();
}

void UnrecognizedOpcodeException::make_message(u16 opcode)
{
    std::stringstream ss;
    ss << runtime_error::what() << ": opcode = " << hexify(opcode);

    m_message = ss.str();
}
}
