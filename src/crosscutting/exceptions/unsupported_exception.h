#pragma once

#include <stdexcept>
#include <string>

namespace emu::exceptions {

class UnsupportedException : public std::invalid_argument {
public:
    explicit UnsupportedException(std::string const& message);

    [[nodiscard]] auto what() const noexcept -> char const* override;

private:
    void make_message(std::string const& message);

    std::string m_message;
};
}
