#ifndef MIKA_EMULATORS_CROSSCUTTING_ROM_FILE_NOT_FOUND_EXCEPTION_H
#define MIKA_EMULATORS_CROSSCUTTING_ROM_FILE_NOT_FOUND_EXCEPTION_H

#include <iostream>
#include <stdexcept>

namespace emu::exceptions {

    class RomFileNotFoundException : public std::runtime_error {
    public:
        explicit RomFileNotFoundException(const std::string &message);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        void make_message(const std::string &message);

        std::string m_message;
    };
}

#endif //MIKA_EMULATORS_CROSSCUTTING_ROM_FILE_NOT_FOUND_EXCEPTION_H
