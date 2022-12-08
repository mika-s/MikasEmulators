#pragma once

#include <stdexcept>
#include <string>

namespace emu::exceptions {

class RomFileNotFoundException : public std::runtime_error {
public:
    explicit RomFileNotFoundException(std::string const& message);

    [[nodiscard]] char const* what() const noexcept override;

private:
    void make_message(std::string const& message);

    std::string m_message;
};
}
