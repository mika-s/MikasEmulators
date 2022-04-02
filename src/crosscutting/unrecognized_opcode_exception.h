#ifndef MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H
#define MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H

#include <iostream>
#include <exception>
#include <stdexcept>

namespace emu::util::exceptions {

    class UnrecognizedOpcodeException : public std::runtime_error {
    public:
        explicit UnrecognizedOpcodeException(std::uint8_t opcode);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        void make_message(std::uint8_t opcode);

        std::string m_message;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_UNRECOGNIZED_OPCODE_EXCEPTION_H
