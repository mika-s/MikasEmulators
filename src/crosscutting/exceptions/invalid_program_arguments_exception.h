#ifndef MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H
#define MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H

#include <functional>
#include <iostream>
#include <stdexcept>

namespace emu::exceptions {

    class InvalidProgramArgumentsException : public std::runtime_error {
    public:
        explicit InvalidProgramArgumentsException(
                const std::string &message,
                std::function<void(const std::string &program_name)> usage_function
        );

        [[nodiscard]] const char *what() const noexcept override;

        [[nodiscard]] const std::function<void(const std::string &program_name)> &usage_function() const;

    private:
        void make_message(const std::string &message);

        std::string m_message;
        std::function<void(const std::string &program_name)> m_usage_function;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_INVALID_PROGRAM_ARGUMENTS_EXCEPTION_H
