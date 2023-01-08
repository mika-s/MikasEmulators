#include "options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"
#include "frontend.h"
#include <ext/alloc_traits.h>
#include <utility>

namespace emu::applications {

using emu::exceptions::InvalidProgramArgumentsException;
using emu::util::string::find_short_executable_name;

Options::Options(std::vector<std::string> args)
    : m_args(args)
{
    m_short_executable_name = find_short_executable_name(args[0]);
}

std::vector<std::string> Options::args() const
{
    return m_args;
}

std::string Options::short_executable_name() const
{
    return m_short_executable_name;
}

GuiType Options::gui_type(std::function<void(std::string const&)> const& print_usage) const
{
    if (!m_options.contains("g")) {
        return GuiType::ORDINARY;
    }
    if (m_options.at("g").size() > 1) {
        throw InvalidProgramArgumentsException("-g flag should only be used once", print_usage);
    } else if (m_options.at("g").empty()) {
        throw InvalidProgramArgumentsException("-g flag needs an additional argument", print_usage);
    }

    if (m_options.at("g")[0] == "ordinary") {
        return GuiType::ORDINARY;
    } else if (m_options.at("g")[0] == "debugging") {
        return GuiType::DEBUGGING;
    } else {
        throw InvalidProgramArgumentsException("Unknown GUI type passed to the -g flag", print_usage);
    }
}

std::pair<bool, std::string> Options::is_asking_for_help() const
{
    return { m_is_asking_for_help, m_is_asking_for_help_reason };
}

void Options::set_is_asking_for_help(std::string reason)
{
    m_is_asking_for_help = true;
    m_is_asking_for_help_reason = std::move(reason);
}

std::string Options::command() const
{
    return m_command;
}

void Options::set_command(std::string command)
{
    m_command = std::move(command);
}

std::optional<std::string> Options::application() const
{
    return m_application;
}

void Options::set_application(std::string application)
{
    m_application = std::optional(application);
}

std::optional<std::string> Options::path() const
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
    if (m_options.count(name) == 0) {
        std::vector<std::string> vec;
        m_options[name] = vec;
        m_is_asking_for_help = m_is_asking_for_help || m_options.contains(s_help_long) || m_options.contains(s_help_short);
        m_is_debugging_cmd_parser = m_is_debugging_cmd_parser || m_options.contains(s_debug_scanner_long);
    }
}

void Options::add_option(std::string const& name, std::string const& value)
{
    if (m_options.count(name) == 0) {
        std::vector<std::string> vec;
        vec.emplace_back(value);
        m_options[name] = vec;
    } else {
        m_options[name].push_back(value);
    }

    m_is_asking_for_help = m_is_asking_for_help || m_options.contains(s_help_long) || m_options.contains(s_help_short);
}

std::unordered_map<std::string, std::vector<std::string>> Options::options() const
{
    return m_options;
}

std::pair<bool, std::string> Options::is_failed() const
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
std::pair<bool, std::vector<std::string>> Options::is_debugging_cmd_parser() const
{
    return { m_is_debugging_cmd_parser, m_cmd_tokens };
}
}
