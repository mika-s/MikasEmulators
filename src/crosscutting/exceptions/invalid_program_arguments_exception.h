#ifndef MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H
#define MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H

#include <iostream>
#include <stdexcept>

namespace emu::exceptions {

    class InvalidProgramArgumentsException : public std::runtime_error {
    public:
        explicit InvalidProgramArgumentsException(const std::string &message);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        void make_message(const std::string &message);

        std::string m_message;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H
