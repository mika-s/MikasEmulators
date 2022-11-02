#include "options.h"
#include "crosscutting/exceptions/invalid_program_arguments_exception.h"
#include "crosscutting/util/string_util.h"
#include "frontend.h"

namespace emu::applications {

    using emu::exceptions::InvalidProgramArgumentsException;
    using emu::util::string::find_short_executable_name;

    Options::Options(std::vector<std::string> args, std::string short_executable_name)
        : m_args(args),
          m_is_asking_for_help(false),
          m_short_executable_name(std::move(short_executable_name)) {

        for (std::size_t arg_idx = 0; arg_idx < args.size(); ++arg_idx) {
            if (args[arg_idx] == dipswitch_flag) {
                parse_flag(dipswitch_flag, args, arg_idx);
            } else if (args[arg_idx] == gui_flag) {
                parse_flag(gui_flag, args, arg_idx);
            } else if (args[arg_idx] == help_flag_short || args[arg_idx] == help_flag_long) {
                m_is_asking_for_help = true;
            }
        }
    }

    std::vector<std::string> Options::args() {
        return m_args;
    }

    std::string Options::short_executable_name() {
        return m_short_executable_name;
    }

    GuiType Options::gui_type(const std::function<void(const std::string &)>& print_usage) {
        if (!m_options.contains("-g")) {
            return GuiType::ORDINARY;
        }
        if (m_options["-g"].size() > 1) {
            throw InvalidProgramArgumentsException("-g flag should only be used once", print_usage);
        } else if (m_options["-g"].empty()) {
            throw InvalidProgramArgumentsException("-g flag needs an additional argument", print_usage);
        }

        if (m_options["-g"][0] == "ordinary") {
            return GuiType::ORDINARY;
        } else if (m_options["-g"][0] == "debugging") {
            return GuiType::DEBUGGING;
        } else {
            throw InvalidProgramArgumentsException("Unknown GUI type passed to the -g flag", print_usage);
        }
    }

    bool Options::is_asking_for_help() {
        return m_is_asking_for_help;
    }

    void Options::parse_flag(const std::string &flag, std::vector<std::string> args, std::size_t arg_idx) {
        if (arg_idx == args.size() - 1) {
            throw InvalidProgramArgumentsException(
                    flag + " flag at the end of the line, without a value.", &Frontend::print_run_usage
            );
        }

        if (m_options.count(flag) == 0) {
            std::vector<std::string> vec;
            vec.emplace_back(args[arg_idx + 1]);
            m_options[flag] = vec;
        } else {
            m_options[flag].push_back(args[arg_idx + 1]);
        }
    }
    std::unordered_map<std::string, std::vector<std::string>> Options::options() {
        return m_options;
    }
}
