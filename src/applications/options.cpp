#include "options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"
#include "frontend.h"
#include <utility>

namespace emu::applications {

using emu::exceptions::InvalidProgramArgumentsException;
using emu::util::string::find_short_executable_name;

Options::Options(const std::vector<std::string> &args)
    : m_args(args)
{
    m_short_executable_name = find_short_executable_name(args.at(0));
}

auto Options::args() const -> std::vector<std::string>
{
    return m_args;
}

auto Options::short_executable_name() const -> std::string
{
    return m_short_executable_name;
}

auto Options::gui_type(std::function<void(std::string const&)> const& print_usage) const -> GuiType
{
    if (!m_options.contains("g")) {
        return GuiType::ORDINARY;
    }
    if (m_options.at("g").size() > 1) {
        throw InvalidProgramArgumentsException("-g flag should only be used once", print_usage);
    }
    if (m_options.at("g").empty()) {
        throw InvalidProgramArgumentsException("-g flag needs an additional argument", print_usage);
    }
    if (m_options.at("g").at(0) == "ordinary") {
        return GuiType::ORDINARY;
    }
    if (m_options.at("g").at(0) == "debugging") {
        return GuiType::DEBUGGING;
    }

    throw InvalidProgramArgumentsException("Unknown GUI type passed to the -g flag", print_usage);
}

auto Options::is_asking_for_help() const -> std::pair<bool, std::string>
{
    return { m_is_asking_for_help, m_is_asking_for_help_reason };
}

void Options::set_is_asking_for_help(std::string reason)
{
    m_is_asking_for_help = true;
    m_is_asking_for_help_reason = std::move(reason);
}

auto Options::command() const -> std::string
{
    return m_command;
}

void Options::set_command(std::string command)
{
    m_command = std::move(command);
}

auto Options::application() const -> std::optional<std::string>
{
    return m_application;
}

void Options::set_application(std::string application)
{
    m_application = std::optional(application);
}

auto Options::path() const -> std::optional<std::string>
{
    return m_path;
}

void Options::set_path(std::string path)
{
    m_path = std::optional(path);
}

void Options::set_tokens(std::vector<std::string> tokens)
{
    m_cmd_tokens = std::move(tokens);
}

void Options::add_option(std::string const& name)
{
    if (!m_options.contains(name)) {
        constexpr std::vector<std::string> vec;
        m_options[name] = vec;
        m_is_asking_for_help = m_is_asking_for_help || m_options.contains(s_help_long) || m_options.contains(s_help_short);
        m_is_debugging_cmd_parser = m_is_debugging_cmd_parser || m_options.contains(s_debug_scanner_long);
    }
}

void Options::add_option(std::string const& name, std::string const& value)
{
    if (!m_options.contains(name)) {
        std::vector<std::string> vec;
        vec.emplace_back(value);
        m_options[name] = vec;
    } else {
        m_options[name].push_back(value);
    }

    m_is_asking_for_help = m_is_asking_for_help || m_options.contains(s_help_long) || m_options.contains(s_help_short);
}

auto Options::options() const -> std::unordered_map<std::string, std::vector<std::string>>
{
    return m_options;
}

auto Options::is_failed() const -> std::pair<bool, std::string>
{
    return { m_is_failed, m_failed_reason };
}

void Options::fail(std::string reason)
{
    if (!m_is_failed) {
        m_is_failed = true;

        m_failed_reason = std::move(reason);
    }
}
auto Options::is_debugging_cmd_parser() const -> std::pair<bool, std::vector<std::string>>
{
    return { m_is_debugging_cmd_parser, m_cmd_tokens };
}
}
