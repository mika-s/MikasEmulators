#pragma once

#include <stdexcept>
#include <string>

namespace emu::lmc {

class UnrecognizedAssemblyException : public std::runtime_error {
public:
    explicit UnrecognizedAssemblyException(std::string const& assembly);

    UnrecognizedAssemblyException(std::string const& assembly, std::string const& extra_message);

    [[nodiscard]] auto what() const noexcept -> char const* override;

private:
    void make_message(std::string const& assembly);

    std::string m_message;
};
}
