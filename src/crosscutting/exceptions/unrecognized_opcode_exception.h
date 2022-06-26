#ifndef MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H
#define MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include "crosscutting/typedefs.h"

namespace emu::exceptions {

    class UnrecognizedOpcodeException : public std::runtime_error {
    public:
        explicit UnrecognizedOpcodeException(u8 opcode);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        void make_message(u8 opcode);

        std::string m_message;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H
