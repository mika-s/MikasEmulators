#pragma once

#include "crosscutting/gui/gui_type.h"
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace emu::applications {

using emu::gui::GuiType;

class Options {
public:
    explicit Options(std::vector<std::string> args);

    std::vector<std::string> args() const;

    std::string short_executable_name() const;

    GuiType gui_type(std::function<void(const std::string&)> const& print_usage) const;

    std::pair<bool, std::string> is_asking_for_help() const;

    void set_is_asking_for_help(std::string reason);

    std::string command() const;

    void set_command(std::string command);

    std::optional<std::string> application() const;

    void set_application(std::string command);

    std::optional<std::string> path() const;

    void set_path(std::string path);

    void set_tokens(std::vector<std::string> tokens);

    void add_option(std::string const& name);

    void add_option(std::string const& name, std::string const& value);

    std::unordered_map<std::string, std::vector<std::string>> options() const;

    std::pair<bool, std::string> is_failed() const;

    std::pair<bool, std::vector<std::string>> is_debugging_cmd_parser() const;

    void fail(std::string reason);

private:
    std::vector<std::string> m_args { "" };
    std::string m_command { "" };
    std::optional<std::string> m_application;
    std::optional<std::string> m_path;
    bool m_is_asking_for_help { false };
    std::string m_is_asking_for_help_reason { "" };
    bool m_is_failed { false };
    std::string m_failed_reason { "" };
    bool m_is_debugging_cmd_parser { false };
    std::vector<std::string> m_cmd_tokens;
    std::string m_short_executable_name { "" };
    std::unordered_map<std::string, std::vector<std::string>> m_options;
};
}
