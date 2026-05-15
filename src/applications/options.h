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
    explicit Options(const std::vector<std::string> &args);

    auto args() const -> std::vector<std::string>;

    auto short_executable_name() const -> std::string;

    auto gui_type(std::function<void(std::string const&)> const& print_usage) const -> GuiType;

    auto is_asking_for_help() const -> std::pair<bool, std::string>;

    void set_is_asking_for_help(std::string reason);

    auto command() const -> std::string;

    void set_command(std::string command);

    auto application() const -> std::optional<std::string>;

    void set_application(std::string application);

    auto path() const -> std::optional<std::string>;

    void set_path(std::string path);

    void set_tokens(std::vector<std::string> tokens);

    void add_option(std::string const& name);

    void add_option(std::string const& name, std::string const& value);

    auto options() const -> std::unordered_map<std::string, std::vector<std::string>>;

    auto is_failed() const -> std::pair<bool, std::string>;

    auto is_debugging_cmd_parser() const -> std::pair<bool, std::vector<std::string>>;

    void fail(std::string reason);

private:
    static const inline std::string s_help_short = "h";
    static const inline std::string s_help_long = "help";
    static const inline std::string s_debug_scanner_long = "debug-scanner";

    std::vector<std::string> m_args { "" };
    std::string m_command;
    std::optional<std::string> m_application;
    std::optional<std::string> m_path;
    bool m_is_asking_for_help { false };
    std::string m_is_asking_for_help_reason;
    bool m_is_failed { false };
    std::string m_failed_reason;
    bool m_is_debugging_cmd_parser { false };
    std::vector<std::string> m_cmd_tokens;
    std::string m_short_executable_name;
    std::unordered_map<std::string, std::vector<std::string>> m_options;
};
}
