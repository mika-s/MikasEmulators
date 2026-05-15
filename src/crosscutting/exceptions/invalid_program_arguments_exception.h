#pragma once

#include <functional>
#include <stdexcept>
#include <string>

namespace emu::exceptions {

class InvalidProgramArgumentsException : public std::runtime_error {
public:
    explicit InvalidProgramArgumentsException(
        std::string const& msg,
        std::function<void(std::string const& program_name)> usage_function);

    [[nodiscard]] auto what() const noexcept -> char const* override;

    [[nodiscard]] auto usage_function() const -> std::function<void(const std::string& program_name)> const&;

private:
    void make_message(std::string const& msg);

    std::string m_message;
    std::function<void(std::string const& program_name)> m_usage_function;
};
}
