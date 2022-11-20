#ifndef MIKA_EMULATORS_CHIPS_LMC_UNRECOGNIZED_ASSEMBLY_EXCEPTION_H
#define MIKA_EMULATORS_CHIPS_LMC_UNRECOGNIZED_ASSEMBLY_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace emu::lmc {

    class UnrecognizedAssemblyException : public std::runtime_error {
    public:
        explicit UnrecognizedAssemblyException(const std::string &assembly);

        UnrecognizedAssemblyException(const std::string &assembly, const std::string &extra_message);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        void make_message(const std::string &assembly);

        std::string m_message;
    };
}

#endif //MIKA_EMULATORS_CHIPS_LMC_UNRECOGNIZED_ASSEMBLY_EXCEPTION_H
